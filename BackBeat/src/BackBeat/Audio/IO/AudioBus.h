#pragma once

// NOTE: Implemented but not currently used in application. Here for possible standardization with other audio plugins

#include "AudioBuffer.h"
#include "Bus.h"
namespace BackBeat {

	class AudioBus : public Bus
	{
	public:
		AudioBus(AudioProps props, BusDirection direction);
		~AudioBus();

		inline AudioBuffer* GetBuffer() { return &m_Buffer; }

		inline virtual unsigned int GetBufferSize() override { return m_Buffer.GetSize(); }
		inline virtual BusDirection GetDirection() override { return m_Direction; }

	private:
		BusDirection m_Direction;
		AudioProps m_Props;
		AudioBuffer m_Buffer;
	};
}