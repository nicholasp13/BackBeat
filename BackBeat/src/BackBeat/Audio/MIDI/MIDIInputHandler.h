#pragma once

/**
*  Wrapper over RenderInfo so that only MIDI events can be inputted without exposing RenderInfo
*/

#include "BackBeat/Audio/IO/RenderInfo.h"
namespace BackBeat {

	class MIDIInputHandler
	{
	public:
		MIDIInputHandler(std::shared_ptr<RenderInfo> renderInfo);
		~MIDIInputHandler();

		void Push(MIDIEvent event);
		void ClearEvent();

		inline bool IsKeyPresssed() { return m_LastPressedEvent.status != MIDI::NoteOff; }
		inline MIDIEvent GetLastEvent() { return m_LastPressedEvent; }
		inline MIDICode GetLastKeyPressed() { return m_LastPressedEvent.data1; }

	private:
		MIDIEvent m_LastPressedEvent;
		std::shared_ptr<RenderInfo> m_RenderInfo;

	};
}