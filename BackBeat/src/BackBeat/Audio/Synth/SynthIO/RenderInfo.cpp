#include "bbpch.h"

#include "RenderInfo.h"
namespace BackBeat {

	RenderInfo::RenderInfo(AudioBuffer* buffer)
		: 
		m_SamplesToRender(0), 
		m_Buffer(buffer)
	{

	}

	RenderInfo::~RenderInfo()
	{

	}

	MIDIEvent RenderInfo::PopMIDIEvent()
	{
		MIDIEvent event = {
			.status = (byte)0x00,
			.data1 =  (byte)0x00,
			.data2 =  (byte)0x00
		};

		if (m_MIDIEvents.empty())
			return event;
		event = m_MIDIEvents.front();
		m_MIDIEvents.pop();
		return event;
	}
}