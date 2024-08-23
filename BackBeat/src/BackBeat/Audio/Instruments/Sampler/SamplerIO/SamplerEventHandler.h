#pragma once

#include <queue>
#include <map>

#include "BackBeat/Audio/Instruments/Sampler/SamplerIO/SamplerPad.h"
#include "BackBeat/Events/KeyEvent.h"
namespace BackBeat {

	class SamplerEventHandler
	{
	public:
		SamplerEventHandler();
		~SamplerEventHandler();

		bool HandleEvent(Event& event);
		bool AddSamplerPad(std::shared_ptr<SamplerPad> newPad, KeyCode key);
		bool ChangeKeyCode(KeyCode oldKey, KeyCode newKey);
		bool ChangeKeyCodeIndex(unsigned int index, KeyCode newKey);

	private:
		std::vector < std::pair<std::shared_ptr<SamplerPad>, KeyCode> > m_SamplerPads;

		bool OnKeyPressed(KeyPressedEvent& event);
		bool OnKeyReleased(KeyReleasedEvent& event);
	};

}