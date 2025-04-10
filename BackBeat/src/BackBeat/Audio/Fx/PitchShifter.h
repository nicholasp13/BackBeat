#pragma once

// Uses Will Pirkle's peak finding and locking algorithms found in PSVocoder
// This is essentially PSVocoder
// link: https://www.willpirkle.com/fx-book/project-gallery/

#include "PhaseVocoder.h"
namespace BackBeat {

	constexpr unsigned int PitchShifterFFTLength = 4096;

	struct PitchShifterParameters
	{
		float shiftSemitones = 0.0f; // -12.0f -> 12.0f | 1 octave down to one octave up

		PitchShifterParameters& operator = (const PitchShifterParameters& rhs)
		{
			shiftSemitones = rhs.shiftSemitones;
			return *this;
		}

		bool operator == (const PitchShifterParameters& rhs)
		{
			const float delta = 0.001f; // A thousandth of a second difference
			return Audio::EqualsFloat(shiftSemitones, rhs.shiftSemitones, delta);
		}

		bool operator != (const PitchShifterParameters& rhs)
		{
			return !(*this == rhs);
		}

	};

	class PitchShifter
	{
	public:
		PitchShifter();
		~PitchShifter();
		
		void Update(PitchShifterParameters params);
		void ResetOutput();
		void ProcessAudioFrame(float* input, unsigned int numSamples);

		inline unsigned int GetFFTSize() { return PitchShifterFFTLength; }
		inline unsigned int GetInputHopSize() { return m_InputHopSize; }
		inline unsigned int GetOutputHopSize() { return m_OutputHopSize; }
		inline unsigned int GetOutputSize() { return m_OutputSize; }
		inline float* GetOutput() { return m_Output; }
		inline float* GetWindowBuffer() { return m_WindowBuffer; }

	private:
		static const unsigned int m_MaxOutputSize = 960000; // 20 seconds in 48k sample rate
		const unsigned int m_InputHopSize = (PitchShifterFFTLength / 4);
		const WindowType m_WindowType = WindowType::Hann;

		bool m_Init = false;
		unsigned int m_OutputResampleLength = 0;
		unsigned int m_OutputSize = 0;
		unsigned int m_OutputIndex = 0;
		unsigned int m_OutputHopSize = (PitchShifterFFTLength / 4);
		float m_WindowCorrectionGain = 0.0f;
		float m_Ratio = 0.0f;

		int m_PeakBins[PitchShifterFFTLength] = { -1 };         // array of current peak bin index values (-1 = not peak)
		int m_PeakBinsPrevious[PitchShifterFFTLength] = { -1 }; // array of previous peak bin index values (-1 = not peak)
		float m_Phi[PitchShifterFFTLength] = { 0.0f };          // array of phase values for classic algorithm
		float m_Psi[PitchShifterFFTLength] = { 0.0f };          // array of phase correction values for classic algorithm        
		float m_IFFTOutput[PitchShifterFFTLength] = { 0.0f };
		float m_Output[m_MaxOutputSize] = { 0.0f };
		float* m_WindowBuffer = nullptr;
		float* m_ResampleBuffer = nullptr;

		complexArray m_FFTOutput = complexArray(PitchShifterFFTLength);
		complexArray m_IFFTInput = complexArray(PitchShifterFFTLength);
		PitchShifterParameters m_Params = PitchShifterParameters();
		BinData m_BinData[PitchShifterFFTLength] = {};
		BinData m_BinDataPrevious[PitchShifterFFTLength] = {};

	private:
		int FindPreviousNearestPeak(unsigned int index);
		void FindPeaksAndRegionsOfInfluence();
		void AddZeroPad(float* buffer, unsigned int num);
		void OverlapAdd(float* buffer, unsigned int num);
	};

}