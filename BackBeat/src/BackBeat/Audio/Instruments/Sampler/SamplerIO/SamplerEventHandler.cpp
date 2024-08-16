#include "bbpch.h"

// NOTE: Defaukt key controls are 1 - 9, 0 keys

#include "SamplerEventHandler.h"
namespace BackBeat {

	SamplerEventHandler::SamplerEventHandler()
	{

	}

	SamplerEventHandler::~SamplerEventHandler()
	{

	}

	bool SamplerEventHandler::HandleEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_FN(SamplerEventHandler::OnKeyPressed));
		dispatcher.Dispatch<KeyReleasedEvent>(BIND_EVENT_FN(SamplerEventHandler::OnKeyReleased));
		return true;
	}

	bool SamplerEventHandler::AddSamplerPad(std::shared_ptr<SamplerPad> newPad, KeyCode key)
	{
		m_SamplerPads.push_back(std::make_pair(newPad, key));
		return true;
	}

	bool SamplerEventHandler::ChangeKeyCode(KeyCode oldKey, KeyCode newKey)
	{
		for (auto itr = m_SamplerPads.begin(); itr != m_SamplerPads.end(); itr++) {
			if (itr->second == oldKey)
			{
				itr->second = newKey;
				return true;
			}
		}
		return false;
	}

	// NOTE: - Implicit that the pads are in a specific order that is never changing. Might need a more explicit way
	//       to specify or get certain pad like a GID or an UID
	//       - Also function overloading does not work as unsigned int and KeyCode are the same type
	bool SamplerEventHandler::ChangeKeyCodeIndex(unsigned int index, KeyCode newKey)
	{
		if (index >= m_SamplerPads.size())
			return false;

		m_SamplerPads[index].second = newKey;
		return true;
	}

	bool SamplerEventHandler::OnKeyPressed(KeyPressedEvent& event)
	{
		if (event.isRepeat())
			return true;

		for (auto itr = m_SamplerPads.begin(); itr != m_SamplerPads.end(); itr++) {
			if (itr->second == event.GetKeyCode())
			{
				itr->first->Press();
				return true;
			}
		}
		return false;
	}

	bool SamplerEventHandler::OnKeyReleased(KeyReleasedEvent& event)
	{
		return true;
	}
}