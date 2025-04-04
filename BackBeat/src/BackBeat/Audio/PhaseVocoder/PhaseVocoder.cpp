#include "PhaseVocoder.h"

namespace BackBeat {

	PhaseVocoder::PhaseVocoder()
	{

	}


	PhaseVocoder::~PhaseVocoder()
	{
		delete[m_FrameLength] m_WindowBuffer;
		delete[m_FrameLength] m_TempOutputBuffer;
		DeleteFFTW();
	}


	void PhaseVocoder::Init(unsigned frameLength, unsigned int hopSize, WindowType windowType)
	{
		if (hopSize > frameLength)
			return;

		// If not first successful Init() call, delete certain members
		if (m_Init)
		{
			delete[m_FrameLength] m_WindowBuffer;
			delete[m_FrameLength] m_TempOutputBuffer;
			DeleteFFTW();
		}

		ResetOutput();

		m_FrameLength = frameLength;
		m_HopSize = hopSize;
		m_WindowType = windowType;

		m_WindowBuffer = new float[frameLength];
		m_TempOutputBuffer = new float[frameLength];

		InitWindow(m_WindowBuffer, m_FrameLength, m_HopSize, m_WindowType, m_WindowCorrectionGain);

		// Allocate fftw members
		m_FFTInput = (fftwf_complex*)fftwf_malloc(sizeof(fftwf_complex) * m_FrameLength);
		m_FFTOutput = (fftwf_complex*)fftwf_malloc(sizeof(fftwf_complex) * m_FrameLength);
		m_IFFTOutput = (fftwf_complex*)fftwf_malloc(sizeof(fftwf_complex) * m_FrameLength);
		m_FFTPlan = fftwf_plan_dft_1d(m_FrameLength, m_FFTInput, m_FFTOutput, FFTW_FORWARD, FFTW_ESTIMATE);
		m_IFFTPlan = fftwf_plan_dft_1d(m_FrameLength, m_FFTOutput, m_IFFTOutput, FFTW_BACKWARD, FFTW_ESTIMATE);

		m_Init = true;
	}

	void PhaseVocoder::ResetOutput()
	{
		m_OutputSize = 0;
		m_OutputWriteIndex = 0;
		Audio::FlushBuffer((byte*)m_Output, m_MaxOutputSize * Audio::FloatByteSize);
	}

	void PhaseVocoder::ProcessAudioFrame(float* input, unsigned int numSamples)
	{
		if (numSamples > m_FrameLength)
			return;

		if (numSamples < m_FrameLength)
			AddZeroPad(input, m_FrameLength - numSamples);

		// Copy input to in[index][REAL]
		for (unsigned int i = 0; i < numSamples; i++)
		{
			m_FFTInput[i][Real] = input[i];
			m_FFTInput[i][Imaginary] = 0.0f;
		}

		DoFFTW();
	}

	void PhaseVocoder::DoFFTW()
	{
		if (m_Init)
			fftwf_execute(m_FFTPlan);
	}

	void PhaseVocoder::DoIFFT()
	{
		if (!m_Init)
			return;

		fftwf_execute(m_IFFTPlan);

		for (unsigned int i = 0; i < m_FrameLength; i++)
		{
			m_TempOutputBuffer[i] = m_IFFTOutput[i][Real];
		}

		OverlapAdd(m_TempOutputBuffer, m_FrameLength);
	}

	void PhaseVocoder::DeleteFFTW()
	{
		if (m_FFTPlan)
			fftwf_destroy_plan(m_FFTPlan);
		if (m_IFFTPlan)
			fftwf_destroy_plan(m_IFFTPlan);
		if (m_FFTInput)
			fftwf_free(m_FFTInput);
		if (m_FFTOutput)
			fftwf_free(m_FFTOutput);
		if (m_IFFTOutput)
			fftwf_free(m_IFFTOutput);
	}

	void PhaseVocoder::AddZeroPad(float* buffer, unsigned int num)
	{
		for (unsigned int i = m_FrameLength - num; i < m_FrameLength; i++)
		{
			buffer[i] = 0.0f;
		}
	}

	void PhaseVocoder::OverlapAdd(float* buffer, unsigned int num)
	{
		for (unsigned int i = 0; i < num; i++)
		{
			m_Output[m_OutputWriteIndex + i] += (buffer[i] * m_WindowCorrectionGain * m_WindowBuffer[i]);
		}

		m_OutputWriteIndex += m_HopSize;

		if (m_OutputSize == 0)
			m_OutputSize = m_FrameLength;
		else
			m_OutputSize += m_HopSize;
	}

}