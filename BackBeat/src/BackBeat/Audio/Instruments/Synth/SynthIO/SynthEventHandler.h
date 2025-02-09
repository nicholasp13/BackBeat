#pragma once

#include <queue>
#include <map>

#include "BackBeat/Audio/Instruments/Synth/SynthBase.h"
#include "BackBeat/Audio/MIDI/MIDICodes.h"
#include "BackBeat/Events/KeyEvent.h"
#include "BackBeat/Audio/Renderer/RenderInfo.h"
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
		std::map<KeyCode, MIDIEvent> m_ReleaseMap;
		std::shared_ptr<RenderInfo> m_RenderInfo;
		std::shared_ptr<EventHandlerParameters> m_Params;

	private:
		void InitMIDIMap();
		bool OnKeyPressed(KeyPressedEvent& event);
		bool OnKeyReleased(KeyReleasedEvent& event);
	};
}