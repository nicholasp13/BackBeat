#pragma once

#include <queue>
#include <map>
#include "BackBeat/Audio/Synth/SynthBase.h"
#include "BackBeat/Events/KeyEvent.h"
#include "BackBeat/Audio/Synth/Render/RenderInfo.h"
// TODO:
// * Add ability to change volume from events (Only applies to Synth)
namespace BackBeat {

	class SynthEventHandler
	{
	public:
		SynthEventHandler(std::shared_ptr<RenderInfo> renderInfo);
		~SynthEventHandler();

		bool HandleEvent(Event& event);
		bool AddEvent(KeyCode key, midiEvent note);

		void SetVelocity(byte velocity) { m_Velocity = velocity; }

	private:
		int m_Octave;
		byte m_Velocity;

		std::map<KeyCode, midiEvent> m_MIDIMap;
		std::shared_ptr<RenderInfo> m_RenderInfo;

		void InitMIDIMap();
		bool OnKeyPressed(KeyPressedEvent& event);
		bool OnKeyReleased(KeyReleasedEvent& event);
	};
}