#pragma once

#include "BackBeat/Audio/Synth/SynthBase.h"
namespace BackBeat {

	class Modulator
	{
	public:
		Modulator(UINT32 bufferSize);
		~Modulator();

		void FlushBuffer(UINT32 numSamples, float defaultValue);

		std::shared_ptr<float[]> GetBuffer() { return m_Buffer; }
		
	private:
		UINT32 m_BufferSize;
		std::shared_ptr<float[]> m_Buffer;
	};
}