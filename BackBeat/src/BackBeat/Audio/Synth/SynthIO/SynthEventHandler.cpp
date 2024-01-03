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
			MIDIEvent(MIDI::ChannelOn_1, MIDI::C_4, MIDI::MaxVelocity));
		AddEvent(Key::S,
			MIDIEvent(MIDI::ChannelOn_1, MIDI::D_4, MIDI::MaxVelocity));
		AddEvent(Key::D,
			MIDIEvent(MIDI::ChannelOn_1, MIDI::E_4, MIDI::MaxVelocity));
		AddEvent(Key::F,
			MIDIEvent(MIDI::ChannelOn_1, MIDI::F_4, MIDI::MaxVelocity));
		AddEvent(Key::G,
			MIDIEvent(MIDI::ChannelOn_1, MIDI::G_4, MIDI::MaxVelocity));
		AddEvent(Key::H,
			MIDIEvent(MIDI::ChannelOn_1, MIDI::A_4, MIDI::MaxVelocity));
		AddEvent(Key::J,
			MIDIEvent(MIDI::ChannelOn_1, MIDI::B_4, MIDI::MaxVelocity));
		AddEvent(Key::K,
			MIDIEvent(MIDI::ChannelOn_1, MIDI::C_5, MIDI::MaxVelocity));

		// BLACK KEYS: 'WE TYU'
		AddEvent(Key::W,
			MIDIEvent(MIDI::ChannelOn_1, MIDI::CSharp_4, MIDI::MaxVelocity));
		AddEvent(Key::E,
			MIDIEvent(MIDI::ChannelOn_1, MIDI::DSharp_4, MIDI::MaxVelocity));
		AddEvent(Key::T,
			MIDIEvent(MIDI::ChannelOn_1, MIDI::FSharp_4, MIDI::MaxVelocity));
		AddEvent(Key::Y,
			MIDIEvent(MIDI::ChannelOn_1, MIDI::GSharp_4, MIDI::MaxVelocity));
		AddEvent(Key::U,
			MIDIEvent(MIDI::ChannelOn_1, MIDI::ASharp_4, MIDI::MaxVelocity));
	}

	// Note: Does not set event.Handled to true in case application wants to 
	//       pass event to something else like a visualizer for the key pressed.
	bool SynthEventHandler::OnKeyPressed(KeyPressedEvent& event)
	{
		if (m_MIDIMap.find(event.GetKeyCode()) == m_MIDIMap.end())
			return false;
		if (event.isRepeat())
			return false;

		byte octave = (m_Params->octave - 4) * SynthBase::NotesInOctave;

		MIDIEvent mEvent = m_MIDIMap[event.GetKeyCode()];
		mEvent.data1 += octave;
		mEvent.data2 = m_Params->noteVelocity;
		if (mEvent.data1 > MIDI::G_9)
			mEvent.data1 = MIDI::G_9;
		m_RenderInfo->PushMIDIEvent(mEvent);
		return true;
	}

	bool SynthEventHandler::OnKeyReleased(KeyReleasedEvent& event)
	{
		if (m_MIDIMap.find(event.GetKeyCode()) == m_MIDIMap.end())
			return false;

		byte octave = (m_Params->octave - 4) * SynthBase::NotesInOctave;

		MIDIEvent mEvent = m_MIDIMap[event.GetKeyCode()];
		mEvent.status = SynthBase::GetChannelOff(mEvent.status);
		mEvent.data1 += octave;
		mEvent.data2 = m_Params->noteVelocity;
		if (mEvent.data1 > MIDI::G_9)
			mEvent.data1 = MIDI::G_9;
		m_RenderInfo->PushMIDIEvent(mEvent);
		return true;
	}
}