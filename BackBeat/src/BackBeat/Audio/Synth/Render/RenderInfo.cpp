#include "bbpch.h"

#include "RenderInfo.h"
namespace BackBeat {

	RenderInfo::RenderInfo(tWAVEFORMATEX props)
		: 
		m_SamplesToRender(0), 
		m_BufferSize(props.nSamplesPerSec), 
		m_Buffer(std::make_shared<AudioBuffer>(props))
	{

	}

	RenderInfo::~RenderInfo()
	{

	}

	midiEvent RenderInfo::PopMIDIEvent()
	{
		midiEvent event = {
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