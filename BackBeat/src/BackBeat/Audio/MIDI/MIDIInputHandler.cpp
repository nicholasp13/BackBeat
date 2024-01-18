#include "bbpch.h"

#include "MIDIInputHandler.h"
namespace BackBeat {

	MIDIInputHandler::MIDIInputHandler(std::shared_ptr<RenderInfo> renderInfo)
		: m_RenderInfo(renderInfo)
	{
		MIDIEvent event = MIDIEvent();
		event.status = MIDI::NoteOff;
		event.data1 = MIDI::NoteOff;
		event.data2 = MIDI::NoteOff;
		m_LastPressedEvent = event;
	}

	MIDIInputHandler::~MIDIInputHandler()
	{

	}

	void MIDIInputHandler::Push(MIDIEvent event)
	{
		if (Audio::IsNoteOn(event))
			m_LastPressedEvent = event;
		m_RenderInfo->PushMIDIEvent(event);
	}

	void MIDIInputHandler::ClearEvent()
	{
		MIDIEvent event = MIDIEvent();
		event.status = MIDI::NoteOff;
		event.data1 = MIDI::NoteOff;
		event.data2 = MIDI::NoteOff;
		m_LastPressedEvent = event;
	}
}