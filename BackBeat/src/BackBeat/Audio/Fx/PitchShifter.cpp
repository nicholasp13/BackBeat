#include "bbpch.h"

#include "PitchShifter.h"
namespace BackBeat {

	PitchShifter::PitchShifter()
	{

	}

	PitchShifter::~PitchShifter()
	{
		if (m_ResampleBuffer)
			delete[m_OutputResampleLength] m_ResampleBuffer;
		if (m_WindowBuffer)
			delete[m_OutputResampleLength] m_WindowBuffer;
	}

	// Must call at least once before calling ProcessAudioFrame and whenever the user updates params
	void PitchShifter::Update(PitchShifterParameters params)
	{
		if (params.shiftSemitones < -12.0f || params.shiftSemitones > 12.0f)
			return;

		if (m_ResampleBuffer)
			delete[m_OutputResampleLength] m_ResampleBuffer;
		if (m_WindowBuffer)
			delete[m_OutputResampleLength] m_WindowBuffer;

		m_Params = params;
		m_Ratio = pow(2.0f, m_Params.shiftSemitones / 12.0f);
		m_OutputResampleLength = unsigned int(1.0f / m_Ratio * float(PitchShifterFFTLength));
		m_OutputSize = 0;
		m_OutputIndex = 0;
		m_OutputHopSize = unsigned int ((float)m_InputHopSize / m_Ratio);
		m_ResampleBuffer = new float[m_OutputResampleLength];
		m_WindowBuffer = new float[m_OutputResampleLength];

		InitWindow(m_WindowBuffer, m_OutputResampleLength, m_OutputHopSize, m_WindowType, m_WindowCorrectionGain);

		Audio::FlushBuffer((byte*)m_Output, m_MaxOutputSize * Audio::FloatByteSize);
		Audio::FlushBuffer((byte*)m_Phi, PitchShifterFFTLength * Audio::FloatByteSize);
		Audio::FlushBuffer((byte*)m_Psi, PitchShifterFFTLength * Audio::FloatByteSize);

		for (unsigned int i = 0; i < PitchShifterFFTLength; i++)
		{
			m_BinData[i].Reset();
			m_BinDataPrevious[i].Reset();

			m_PeakBins[i] = -1;
			m_PeakBinsPrevious[i] = -1;
		}

		m_Init = true;
	}

	void PitchShifter::ResetOutput()
	{
		m_OutputSize = 0;
		m_OutputIndex = 0;
		Audio::FlushBuffer((byte*)m_Output, m_MaxOutputSize * Audio::FloatByteSize);

		for (unsigned int i = 0; i < PitchShifterFFTLength; i++)
		{
			m_BinData[i].Reset();
			m_BinDataPrevious[i].Reset();
		}
	}

	// Input bufferSize must at least be TimeStretcherFFTLength to avoid read/write access errors
	// numSamples can be less than the bufferSize
	void PitchShifter::ProcessAudioFrame(float* input, unsigned int numSamples)
	{
		if (!m_Init)
			return;
		if (numSamples > PitchShifterFFTLength)
			return;

		if (numSamples < PitchShifterFFTLength)
			AddZeroPad(input, PitchShifterFFTLength - numSamples);

		// Compute fftw3
		Audio::Algorithms::fftw3(input, &m_FFTOutput, PitchShifterFFTLength);

		// Get the magnitudes for searching
		for (unsigned int i = 0; i < PitchShifterFFTLength; i++)
		{
			m_BinData[i].Reset();
			m_PeakBins[i] = -1;

			m_BinData[i].magnitude = Audio::GetMagnitude(m_FFTOutput.realNums[i], m_FFTOutput.imNums[i]);
			m_BinData[i].phi = Audio::GetPhase(m_FFTOutput.realNums[i], m_FFTOutput.imNums[i]);
		}

		FindPeaksAndRegionsOfInfluence();

		// Each bin data should now know its local boss-peak
		// Now propagate phases accordingly
		//
		// FIRST: set PSI angles of bosses
		for (unsigned int i = 0; i < PitchShifterFFTLength; i++)
		{
			float mag = m_BinData[i].magnitude;
			float phi = m_BinData[i].phi;

			// horizontal phase propagation
			// omega = bin frequency(k)
			float omega = Audio::Pi * 2.0f * i / PitchShifterFFTLength;

			// phase deviation is actual - expected phase
			//     = phi_k -(phi(last frame) + wk*ha
			float phaseDev = phi - m_Phi[i] - omega * m_OutputHopSize;

			// Unwrapped phase increment
			float deltaPhi = omega * m_OutputHopSize + Audio::PrincipalArg(phaseDev);

			// Save for next frame
			m_Phi[i] = phi;

			// --- if peak, assume it could have hopped from a different bin
			if (m_BinData[i].isPeak)
			{
				// Calculate new phase based on stretch factor; save phase for next time
				if (m_BinData[i].previousPeakBin < 0)
					m_Psi[i] = Audio::PrincipalArg(m_Psi[i] + deltaPhi * m_Ratio);
				else
					m_Psi[i] = Audio::PrincipalArg(m_Psi[m_BinDataPrevious[i].previousPeakBin] + deltaPhi * m_Ratio);
			}

			// Save peak PSI (new angle)
			m_BinData[i].psi = m_Psi[i];

			// For IFFT
			m_BinData[i].updatedPhase = m_BinData[i].psi;
		}

		// Set non-peaks
		for (unsigned int i = 0; i < PitchShifterFFTLength; i++)
		{
			if (!m_BinData[i].isPeak)
			{
				int myPeakBin = m_BinData[i].localPeakBin;

				float PSI = m_BinData[myPeakBin].psi;
				float phi = m_BinData[myPeakBin].phi;

				// Calculate new phase, locked to boss peak
				m_Psi[i] = Audio::PrincipalArg(PSI - phi - m_BinData[i].phi);
				m_BinData[i].updatedPhase = m_Psi[i];
			}
		}

		for (unsigned int i = 0; i < PitchShifterFFTLength; i++)
		{
			float mag = m_BinData[i].magnitude;

			// Convert back
			m_IFFTInput.realNums[i] = Audio::GetRealCoef(mag, m_BinData[i].updatedPhase);
			m_IFFTInput.imNums[i] = Audio::GetImCoef(mag, m_BinData[i].updatedPhase);

			// Save for next frame
			m_BinDataPrevious[i] = m_BinData[i];
			m_PeakBinsPrevious[i] = m_PeakBins[i];
		}

		// Compute ifftw3
		Audio::Algorithms::ifftw3(&m_IFFTInput, m_IFFTOutput, PitchShifterFFTLength);

		// Resample here
		Audio::Resample(m_IFFTOutput, m_ResampleBuffer, PitchShifterFFTLength, m_OutputResampleLength,
			Interpolation::linear);

		// Add overlap
		OverlapAdd(m_ResampleBuffer, m_OutputResampleLength);
	}

	// Find bin index of nearest peak bin in previous FFT frame
	int PitchShifter::FindPreviousNearestPeak(unsigned int index)
	{
		// First run there are no peaks
		if (m_PeakBinsPrevious[0] == -1)
			return -1;

		int delta = -1;
		int previousPeak = -1;
		for (unsigned int i = 0; i < PitchShifterFFTLength; i++)
		{
			if (m_PeakBinsPrevious[i] < 0)
				break;

			int dist = std::abs((int)index - m_PeakBinsPrevious[i]);
			if (dist > PitchShifterFFTLength / 4)
				break;

			if (i == 0)
			{
				previousPeak = i;
				delta = dist;
			}
			else if (dist < delta)
			{
				previousPeak = i;
				delta = dist;
			}
		}

		return previousPeak;
	}

	void PitchShifter::FindPeaksAndRegionsOfInfluence()
	{
		// Find local maxima in 4-sample window
		float localWindow[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
		int m = 0;
		for (unsigned int i = 0; i < PitchShifterFFTLength; i++)
		{
			if (i == 0)
			{
				localWindow[0] = 0.0f;
				localWindow[1] = 0.0f;
				localWindow[2] = m_BinData[i + 1].magnitude;
				localWindow[3] = m_BinData[i + 2].magnitude;
			}
			else  if (i == 1)
			{
				localWindow[0] = 0.0f;
				localWindow[1] = m_BinData[i - 1].magnitude;
				localWindow[2] = m_BinData[i + 1].magnitude;
				localWindow[3] = m_BinData[i + 2].magnitude;
			}
			else  if (i == PitchShifterFFTLength - 1)
			{
				localWindow[0] = m_BinData[i - 2].magnitude;
				localWindow[1] = m_BinData[i - 1].magnitude;
				localWindow[2] = 0.0f;
				localWindow[3] = 0.0f;
			}
			else  if (i == PitchShifterFFTLength - 2)
			{
				localWindow[0] = m_BinData[i - 2].magnitude;
				localWindow[1] = m_BinData[i - 1].magnitude;
				localWindow[2] = m_BinData[i + 1].magnitude;
				localWindow[3] = 0.0f;
			}
			else
			{
				localWindow[0] = m_BinData[i - 2].magnitude;
				localWindow[1] = m_BinData[i - 1].magnitude;
				localWindow[2] = m_BinData[i + 1].magnitude;
				localWindow[3] = m_BinData[i + 2].magnitude;
			}

			// Found peak bin
			if (m_BinData[i].magnitude > 0.00001f &&
				m_BinData[i].magnitude > localWindow[0]
				&& m_BinData[i].magnitude > localWindow[1]
				&& m_BinData[i].magnitude > localWindow[2]
				&& m_BinData[i].magnitude > localWindow[3])
			{
				m_BinData[i].isPeak = true;
				m_PeakBins[m++] = i;

				// For peak bins, assume that it is part of a previous, moving peak
				m_BinData[i].previousPeakBin = FindPreviousNearestPeak(i);
			}
		}

		// Assign peak bosses
		if (m > 0)
		{
			int n = 0;
			int bossPeakBin = m_PeakBins[n];
			double nextPeak = m_PeakBins[++n];
			int midBoundary = int((nextPeak - (double)bossPeakBin) / 2.0) + bossPeakBin;

			if (nextPeak >= 0)
			{
				for (int i = 0; i < PitchShifterFFTLength; i++)
				{
					if (i <= bossPeakBin)
					{
						m_BinData[i].localPeakBin = bossPeakBin;
					}
					else if (i < midBoundary)
					{
						m_BinData[i].localPeakBin = bossPeakBin;
					}
					else // boundary, calc next set
					{
						bossPeakBin = (int)nextPeak;
						nextPeak = m_PeakBins[++n];
						if (nextPeak > bossPeakBin)
							midBoundary = int((nextPeak - (double)bossPeakBin) / 2.0) + bossPeakBin;
						else // nextPeak == -1
							midBoundary = PitchShifterFFTLength;

						m_BinData[i].localPeakBin = bossPeakBin;
					}
				}
			}
		}
	}

	// Assumes to start writing at m_InputHopSize size - num
	void PitchShifter::AddZeroPad(float* buffer, unsigned int num)
	{
		for (unsigned int i = PitchShifterFFTLength - num; i < PitchShifterFFTLength; i++)
		{
			buffer[i] = 0.0f;
		}
	}

	void PitchShifter::OverlapAdd(float* buffer, unsigned int num)
	{
		if (m_OutputIndex + num >= m_MaxOutputSize)
			return;

		for (unsigned int i = 0; i < num; i++)
		{
			m_Output[m_OutputIndex + i] += (buffer[i] * m_WindowCorrectionGain * m_WindowBuffer[i]);
		}

		m_OutputIndex += m_InputHopSize;

		if (m_OutputSize == 0)
			m_OutputSize = m_OutputResampleLength;
		else
			m_OutputSize += m_InputHopSize;
	}
}