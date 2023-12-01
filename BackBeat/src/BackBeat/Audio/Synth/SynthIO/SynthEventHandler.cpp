#include "bbpch.h"

#include "SynthEventHandler.h"
namespace BackBeat {

	SynthEventHandler::SynthEventHandler(std::shared_ptr<RenderInfo> renderInfo)
		:
		m_RenderInfo(renderInfo),
		m_Params(std::make_shared<EventHandlerParameters>(4, (byte)0x7F)),
		m_MIDIMap(std::map<KeyCode, MIDIEvent>())
	{
		InitMIDIMap();
	}

	SynthEventHandler::~SynthEventHandler()
	{

	}

	bool SynthEventHandler::HandleEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_FN(SynthEventHandler::OnKeyPressed));
		dispatcher.Dispatch<KeyReleasedEvent>(BIND_EVENT_FN(SynthEventHandler::OnKeyReleased));
		return true;
	}
	
	bool SynthEventHandler::AddEvent(KeyCode key, MIDIEvent event)
	{
		m_MIDIMap[key] = event;
		return true;
	}

	// Default setup for one octave MIDI piano on keyboard
	void SynthEventHandler::InitMIDIMap()
	{
		// WHITE KEYS: 'ASDFGHJK'
		AddEvent(Key::A, 
			MIDIEvent(CHANNEL_1_NOTE_ON, MIDI_NOTE_C_4, 0x7f));
		AddEvent(Key::S,
			MIDIEvent(CHANNEL_1_NOTE_ON, MIDI_NOTE_D_4, 0x7f));
		AddEvent(Key::D,
			MIDIEvent(CHANNEL_1_NOTE_ON, MIDI_NOTE_E_4, 0x7f));
		AddEvent(Key::F,
			MIDIEvent(CHANNEL_1_NOTE_ON, MIDI_NOTE_F_4, 0x7f));
		AddEvent(Key::G,
			MIDIEvent(CHANNEL_1_NOTE_ON, MIDI_NOTE_G_4, 0x7f));
		AddEvent(Key::H,
			MIDIEvent(CHANNEL_1_NOTE_ON, MIDI_NOTE_A_4, 0x7f));
		AddEvent(Key::J,
			MIDIEvent(CHANNEL_1_NOTE_ON, MIDI_NOTE_B_4, 0x7f));
		AddEvent(Key::K,
			MIDIEvent(CHANNEL_1_NOTE_ON, MIDI_NOTE_C_5, 0x7f));

		// BLACK KEYS: 'WE TYU'
		AddEvent(Key::W,
			MIDIEvent(CHANNEL_1_NOTE_ON, MIDI_NOTE_C_SHARP_4, 0x7f));
		AddEvent(Key::E,
			MIDIEvent(CHANNEL_1_NOTE_ON, MIDI_NOTE_D_SHARP_4, 0x7f));
		AddEvent(Key::T,
			MIDIEvent(CHANNEL_1_NOTE_ON, MIDI_NOTE_F_SHARP_4, 0x7f));
		AddEvent(Key::Y,
			MIDIEvent(CHANNEL_1_NOTE_ON, MIDI_NOTE_G_SHARP_4, 0x7f));
		AddEvent(Key::U,
			MIDIEvent(CHANNEL_1_NOTE_ON, MIDI_NOTE_A_SHARP_4, 0x7f));
	}

	// Note: Does not set event.Handled to true in case application wants to 
	//       pass event to something else like a visualizer for the key pressed.
	bool SynthEventHandler::OnKeyPressed(KeyPressedEvent& event)
	{
		if (m_MIDIMap.find(event.GetKeyCode()) == m_MIDIMap.end())
			return false;
		if (event.isRepeat())
			return false;

		byte octave = (m_Params->octave - 4) * NOTES_IN_OCTAVE;

		MIDIEvent mEvent = m_MIDIMap[event.GetKeyCode()];
		mEvent.data1 += octave;
		mEvent.data2 = m_Params->noteVelocity;
		if (mEvent.data1 > MIDI_NOTE_G_9)
			mEvent.data1 = MIDI_NOTE_G_9;
		m_RenderInfo->PushMIDIEvent(mEvent);
		return true;
	}

	bool SynthEventHandler::OnKeyReleased(KeyReleasedEvent& event)
	{
		if (m_MIDIMap.find(event.GetKeyCode()) == m_MIDIMap.end())
			return false;

		byte octave = (m_Params->octave - 4) * NOTES_IN_OCTAVE;

		MIDIEvent mEvent = m_MIDIMap[event.GetKeyCode()];
		mEvent.status = SynthBase::GetChannelOff(mEvent.status);
		mEvent.data1 += octave;
		mEvent.data2 = m_Params->noteVelocity;
		if (mEvent.data1 > MIDI_NOTE_G_9)
			mEvent.data1 = MIDI_NOTE_G_9;
		m_RenderInfo->PushMIDIEvent(mEvent);
		return true;
	}
}