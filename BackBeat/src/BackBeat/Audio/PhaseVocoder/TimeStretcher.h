#pragma once

// TODO:
//  - Add rule of 5 to TimeStretcherBinData

// Uses Will Pirkle's peak finding and locking algorithms found in PSVocoder
// This is essentially PSVocoder without the pitch shifting
// link: https://www.willpirkle.com/fx-book/project-gallery/

// Also shrinks time (may change name)
// Does not work in real time. (At some point you either run out of computational space or time) 
// Does not use PhaseVocoder as PhaseVocoder produces the same length input as output
// while this always produces a longer output.

#include "BackBeat/Audio/Helpers/CircularBuffer.h"
#include "PhaseVocoder.h"
namespace BackBeat {

	constexpr unsigned int TimeStretcherFFTLength = 4096;

	struct TimeStretcherParameters
	{
		float ratio = 1.0f; // 0.50f -> 2.0f

		TimeStretcherParameters& operator = (const TimeStretcherParameters& rhs)
		{
			ratio = rhs.ratio;
			return *this;
		}

		bool operator == (const TimeStretcherParameters& rhs)
		{
			const float delta = 0.001f; // A thousandth of a second difference
			return Audio::EqualsFloat(this->ratio, rhs.ratio, delta);
		}

		bool operator != (const TimeStretcherParameters& rhs)
		{
			return !(*this == rhs);
		}

	};

	class TimeStretcher
	{
	public:
		TimeStretcher();
		~TimeStretcher();

		void FindPeaksAndRegionsOfInfluence();
		int FindPreviousNearestPeak(unsigned int index);
		void Update(TimeStretcherParameters params);
		void ResetOutput();
		void ProcessAudioFrame(float* input, unsigned int numSamples);

		inline unsigned int GetFFTSize() { return TimeStretcherFFTLength; }
		inline unsigned int GetInputHopSize() { return m_InputHopSize; }
		inline unsigned int GetOutputHopSize() { return m_OutputHopSize; }
		inline unsigned int GetOutputSize() { return m_OutputSize; }
		inline float* GetOutput() { return m_Output; }
		inline float* GetWindowBuffer() { return m_WindowBuffer; }

	private:
		static const unsigned int m_MaxOutputSize = 960000; // 20 seconds in 48k sample rate
		const unsigned int m_InputHopSize = (TimeStretcherFFTLength / 4);
		const unsigned int m_WindowSize = TimeStretcherFFTLength;
		const WindowType m_WindowType = WindowType::Hann;

		bool m_Init;
		unsigned int m_OutputHopSize;
		unsigned int m_OutputSize;
		unsigned int m_OutputIndex;
		float m_WindowCorrectionGain;
		float m_Ratio;

		int m_PeakBins[TimeStretcherFFTLength] = { -1 };         // array of current peak bin index values (-1 = not peak)
		int m_PeakBinsPrevious[TimeStretcherFFTLength] = { -1 }; // array of previous peak bin index values (-1 = not peak)
		float m_Phi[TimeStretcherFFTLength] = { 0.0f };          // array of phase values for classic algorithm
		float m_Psi[TimeStretcherFFTLength] = { 0.0f };          // array of phase correction values for classic algorithm        
		float m_WindowBuffer[TimeStretcherFFTLength] = { 0.0f };
		float m_IFFTOutput[TimeStretcherFFTLength] = { 0.0f };
		float m_Output[m_MaxOutputSize] = { 0.0f };

		complexArray m_FFTOutput;
		complexArray m_IFFTInput;
		TimeStretcherParameters m_Params;
		BinData m_BinData[TimeStretcherFFTLength] = {};
		BinData m_BinDataPrevious[TimeStretcherFFTLength] = {};

	private:
		void AddZeroPad(float* buffer, unsigned int num);
		void OverlapAdd(float* buffer, unsigned int num);
	};

}