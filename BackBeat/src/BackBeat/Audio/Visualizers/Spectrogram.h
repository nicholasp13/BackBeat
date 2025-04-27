#pragma once

#include "BackBeat/Audio/Audio.h"
namespace BackBeat {

	class Spectrogram
	{
	public:
		Spectrogram(unsigned int bufferSize = 0);
		~Spectrogram();
		Spectrogram(const Spectrogram& other);
		
		Spectrogram& operator = (const Spectrogram& rhs);

		// Gets the magnitude of the frequencies. Should only display first half of buffer, numSamples / 2
		void Get(float* input, float* output, unsigned int numSamples);
		// Normalizes with the max value of output, not really visually interesting
		void GetNormalized(float* input, float* output, unsigned int numSamples);
		// Normalizes with the RMS on fft output, not really visually interesting
		void GetNormalizedRMS(float* input, float* output, unsigned int numSamples);

	private:
		unsigned int m_BufferSize; // Since fft is complex the actual bufferSize of ComplexBuffer is twice this

		complexArray m_ComplexBuffer;
	};

}