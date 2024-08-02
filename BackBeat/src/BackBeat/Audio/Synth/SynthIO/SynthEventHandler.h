#pragma once

#include <queue>
#include <map>

// TODO: Change RenderInfo to MIDIInputHandler

#include "BackBeat/Audio/Synth/SynthBase.h"
#include "BackBeat/Audio/MIDI/MIDICodes.h"
#include "BackBeat/Events/KeyEvent.h"
#include "BackBeat/Audio/IO/RenderInfo.h"
namespace BackBeat {

	class SynthEventHandler
	{
	public:
		SynthEventHandler(std::shared_ptr<RenderInfo> renderInfo);
		~SynthEventHandler();

		bool HandleEvent(Event& event);
		bool AddEvent(KeyCode key, MIDIEvent event);

		inline std::shared_ptr<EventHandlerParameters> GetParams() { return m_Params; }

	private:
		std::map<KeyCode, MIDIEvent> m_MIDIMap;
		std::shared_ptr<RenderInfo> m_RenderInfo;
		std::shared_ptr<EventHandlerParameters> m_Params;

		void InitMIDIMap();
		bool OnKeyPressed(KeyPressedEvent& event);
		bool OnKeyReleased(KeyReleasedEvent& event);
	};
}