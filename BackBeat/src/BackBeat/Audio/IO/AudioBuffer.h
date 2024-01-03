#pragma once

#include "BackBeat/Audio/Audio.h"
namespace BackBeat {

	class AudioBuffer
	{
	public:
		AudioBuffer(unsigned int bufferSize);
		~AudioBuffer();

		unsigned int GetSize() { return m_Size; }
		std::shared_ptr<float[]> GetBuffer() { return m_Buffer; }

	private:
		unsigned int m_Size;
		std::shared_ptr<float[]> m_Buffer;
	};
}