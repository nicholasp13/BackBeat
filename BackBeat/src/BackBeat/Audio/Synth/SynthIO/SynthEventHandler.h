#pragma once

#include <queue>
#include <map>

#include "BackBeat/Audio/Synth/SynthBase.h"
#include "BackBeat/Events/KeyEvent.h"
#include "RenderInfo.h"
namespace BackBeat {

	class SynthEventHandler
	{
	public:
		SynthEventHandler(std::shared_ptr<RenderInfo> renderInfo);
		~SynthEventHandler();

		std::shared_ptr<EventHandlerParameters> GetParams() { return m_Params; }

		bool HandleEvent(Event& event);
		bool AddEvent(KeyCode key, MIDIEvent note);

	private:

		std::map<KeyCode, MIDIEvent> m_MIDIMap;
		std::shared_ptr<RenderInfo> m_RenderInfo;
		std::shared_ptr<EventHandlerParameters> m_Params;

		void InitMIDIMap();
		bool OnKeyPressed(KeyPressedEvent& event);
		bool OnKeyReleased(KeyReleasedEvent& event);
	};
}