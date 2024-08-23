#pragma once

#include "BackBeat/Audio/Instruments/Synth/SynthBase.h"
namespace BackBeat {

	class Modulator
	{
	public:
		Modulator(unsigned int bufferSize);
		~Modulator();

		void FlushBuffer(unsigned int numSamples, float defaultValue);

		inline std::shared_ptr<float[]> GetBuffer() { return m_Buffer; }
		
	private:
		unsigned int m_BufferSize;
		std::shared_ptr<float[]> m_Buffer;
	};
}