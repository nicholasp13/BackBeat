#pragma once

#include "AudioBuffer.h"
#include "Bus.h"
namespace BackBeat {

	class AudioBus : public Bus
	{
	public:
		AudioBus(AudioProps props, BusDirection direction);
		~AudioBus();

		AudioBuffer* GetBuffer() { return &m_Buffer; }
		unsigned int GetBufferSize() { return m_Buffer.GetSize(); }

		virtual BusDirection GetDirection() { return m_Direction; }

	private:
		BusDirection m_Direction;
		AudioProps m_Props;
		AudioBuffer m_Buffer;
	};
}