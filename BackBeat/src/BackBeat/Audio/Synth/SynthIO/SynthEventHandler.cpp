#include "bbpch.h"

#include "SynthEventHandler.h"
namespace BackBeat {

	SynthEventHandler::SynthEventHandler(std::shared_ptr<RenderInfo> renderInfo)
		:
		m_RenderInfo(renderInfo),
		m_Params(std::make_shared<EventHandlerParameters>(4, (byte)0x7F)),
		m_MIDIMap(std::map<KeyCode, midiEvent>())
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
	
	bool SynthEventHandler::AddEvent(KeyCode key, midiEvent event)
	{
		m_MIDIMap[key] = event;
		return true;
	}

	// Default setup for one octave MIDI piano on keyboard
	void SynthEventHandler::InitMIDIMap()
	{
		// WHITE KEYS: 'ASDFGHJK'
		AddEvent(Key::A, 
			midiEvent(CHANNEL_1_NOTE_ON, MIDI_NOTE_C_4, 0x7f));
		AddEvent(Key::S,
			midiEvent(CHANNEL_2_NOTE_ON, MIDI_NOTE_D_4, 0x7f));
		AddEvent(Key::D,
			midiEvent(CHANNEL_3_NOTE_ON, MIDI_NOTE_E_4, 0x7f));
		AddEvent(Key::F,
			midiEvent(CHANNEL_4_NOTE_ON, MIDI_NOTE_F_4, 0x7f));
		AddEvent(Key::G,
			midiEvent(CHANNEL_5_NOTE_ON, MIDI_NOTE_G_4, 0x7f));
		AddEvent(Key::H,
			midiEvent(CHANNEL_6_NOTE_ON, MIDI_NOTE_A_4, 0x7f));
		AddEvent(Key::J,
			midiEvent(CHANNEL_7_NOTE_ON, MIDI_NOTE_B_4, 0x7f));
		AddEvent(Key::K,
			midiEvent(CHANNEL_8_NOTE_ON, MIDI_NOTE_C_5, 0x7f));

		// BLACK KEYS: 'WE TYU'
		AddEvent(Key::W,
			midiEvent(CHANNEL_9_NOTE_ON, MIDI_NOTE_C_SHARP_4, 0x7f));
		AddEvent(Key::E,
			midiEvent(CHANNEL_10_NOTE_ON, MIDI_NOTE_D_SHARP_4, 0x7f));
		AddEvent(Key::T,
			midiEvent(CHANNEL_11_NOTE_ON, MIDI_NOTE_F_SHARP_4, 0x7f));
		AddEvent(Key::Y,
			midiEvent(CHANNEL_12_NOTE_ON, MIDI_NOTE_G_SHARP_4, 0x7f));
		AddEvent(Key::U,
			midiEvent(CHANNEL_13_NOTE_ON, MIDI_NOTE_A_SHARP_4, 0x7f));
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

		midiEvent mEvent = m_MIDIMap[event.GetKeyCode()];
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

		midiEvent mEvent = m_MIDIMap[event.GetKeyCode()];
		mEvent.status = SynthBase::GetChannelOff(mEvent.status);
		mEvent.data1 = 0x00;
		mEvent.data2 = 0x00;
		m_RenderInfo->PushMIDIEvent(mEvent);
		return true;
	}
}