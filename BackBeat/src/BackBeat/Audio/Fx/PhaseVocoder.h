#pragma once

// This processes in audio frames which may cause a problem if this is needed for
// real time processes like certain audio fx. To fix that this can be implemented
// with a per sample process instead of audio frames and a circular buffer output
// For current uses in BackBeat this is fine for now

#include <fftw3.h>

#include "BackBeat/Audio/Audio.h"
namespace BackBeat {

	enum class WindowType
	{
		None = 0, 
		Rect,
		Hann,
		BlackmanHarris,
		Hamming
	};

	// Taken Will Pirkle from fxObjects
	// link: https://www.willpirkle.com/fx-book/project-gallery/
	static inline void InitWindow(float* windowBuffer, unsigned int windowSize, unsigned int hopSize, WindowType type, float& gainCorrectionValue)
	{
		if (!windowBuffer)
			return;

		Audio::FlushBuffer((byte*)windowBuffer, windowSize * Audio::FloatByteSize);
		gainCorrectionValue = 0.0f;

		for (unsigned int i = 0; i < windowSize; i++)
		{
			switch (type)
			{
				case WindowType::None:
				{
					windowBuffer[i] = 1.0f;
					break;
				}

				case WindowType::Rect:
				{
					if (i >= 1 && i < windowSize)
						windowBuffer[i] = 1.0f;
					break;
				}

				case WindowType::Hann:
				{
					windowBuffer[i] = 0.5f * (1.0f - cos(((float)i * 2.0f * Audio::Pi) / float(windowSize)));
					break;
				}

				case WindowType::BlackmanHarris:
				{
					windowBuffer[i] = (0.42323f - (0.49755f * cos((i * 2.0f * Audio::Pi)) / float(windowSize))
						+ 0.07922f * cos((2.0f * i * 2.0f * Audio::Pi) / float(windowSize)));
					break;
				}
				case WindowType::Hamming:
				{
					if (i != windowSize - 1)
						windowBuffer[i] = 0.54f - 0.46f * cos(((float)i * 2.0f * Audio::Pi) / float(windowSize));
					break;
				}

				default:
				{
					windowBuffer[i] = 1.0f;
					break;
				}
			}

			gainCorrectionValue += windowBuffer[i];
		}

		float overlap = hopSize > 0.0f ? 1.0f - (float)hopSize / (float)windowSize : 0.0f;

		// Calculate gain correction factor
		if (type != WindowType::None)
			gainCorrectionValue = (1.0f - overlap) / gainCorrectionValue;
		else
			gainCorrectionValue = 1.0f / gainCorrectionValue;
	}

	// Bin data for phase vocoder and similar audio objects that allow/need to keep track of peaks/peak locking
	struct BinData
	{
		BinData() {}

		~BinData() {}

		BinData(const BinData& other)
		{
			isPeak = other.isPeak;
			magnitude = other.magnitude;
			phi = other.phi;
			psi = other.psi;
			localPeakBin = other.localPeakBin;
			previousPeakBin = other.previousPeakBin;
			updatedPhase = other.updatedPhase;
		}

		BinData(BinData&& rhs) noexcept
		{
			isPeak = rhs.isPeak;
			magnitude = rhs.magnitude;
			phi = rhs.phi;
			psi = rhs.psi;
			localPeakBin = rhs.localPeakBin;
			previousPeakBin = rhs.previousPeakBin;
			updatedPhase = rhs.updatedPhase;
		}

		BinData& operator = (BinData&& rhs) noexcept
		{
			isPeak = rhs.isPeak;
			magnitude = rhs.magnitude;
			phi = rhs.phi;
			psi = rhs.psi;
			localPeakBin = rhs.localPeakBin;
			previousPeakBin = rhs.previousPeakBin;
			updatedPhase = rhs.updatedPhase;

			return *this;
		}

		BinData& operator = (const BinData& rhs)
		{
			isPeak = rhs.isPeak;
			magnitude = rhs.magnitude;
			phi = rhs.phi;
			psi = rhs.psi;
			localPeakBin = rhs.localPeakBin;
			previousPeakBin = rhs.previousPeakBin;
			updatedPhase = rhs.updatedPhase;

			return *this;
		}

		void Reset()
		{
			isPeak = false;
			magnitude = 0.0f;
			phi = 0.0f;
			psi = 0.0f;
			localPeakBin = 0;
			previousPeakBin = -1; // -1 is flag
			updatedPhase = 0.0f;
		}

		bool isPeak = false;           // flag for peak bins
		float magnitude = 0.0f;        // bin magnitude angle
		float phi = 0.0f;              // bin phase angle
		float psi = 0.0f;              // bin phase correction
		unsigned int localPeakBin = 0; // index of peak-boss
		int previousPeakBin = -1;      // index of peak bin in previous FFT
		float updatedPhase = 0.0f;     // phase update value
	};

	class PhaseVocoder
	{
	public:
		PhaseVocoder();
		~PhaseVocoder();

		void Init(unsigned int frameLength, unsigned int hopSize, WindowType windowType);
		void ResetOutput();
		void ProcessAudioFrame(float* input, unsigned int numSamples);
		void DoFFTW();
		void DoIFFT();

		inline unsigned int GetFFTSize() { return m_FrameLength; }
		inline unsigned int GetHopSize() { return m_HopSize; }
		inline unsigned int GetOutputSize() { return m_OutputSize; }
		inline float* GetOutput() { return m_Output; }
		inline fftwf_complex* GetFFTOutput() { return m_FFTOutput; }
		inline fftwf_complex* GetIFFTOutput() { return m_IFFTOutput; }

	private:
		static const unsigned int m_MaxOutputSize = 960000; // 20 seconds in 48k sample rate

		bool m_Init = false;
		unsigned int m_FrameLength = 0;
		unsigned int m_HopSize = 0;
		unsigned int m_OutputSize = 0;
		unsigned int m_OutputWriteIndex = 0;
		float m_WindowCorrectionGain = 0.0f;

		float* m_WindowBuffer = nullptr;
		float* m_TempOutputBuffer = nullptr;
		float m_Output[m_MaxOutputSize] = { 0.0f };

		// fftw3 members
		fftwf_complex* m_FFTInput = nullptr;
		fftwf_complex* m_FFTOutput = nullptr;
		fftwf_complex* m_IFFTOutput = nullptr;
		fftwf_plan m_FFTPlan = nullptr;
		fftwf_plan m_IFFTPlan = nullptr;

		WindowType m_WindowType = WindowType::None;

	private:
		void DeleteFFTW();
		void AddZeroPad(float* buffer, unsigned int num);
		void OverlapAdd(float* buffer, unsigned int num);
	};

}