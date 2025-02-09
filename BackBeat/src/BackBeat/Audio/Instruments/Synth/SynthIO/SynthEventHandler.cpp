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
			MIDIEvent(MIDI::ChannelOn1, MIDI::C4, MIDI::MaxVelocity));
		AddEvent(Key::S,
			MIDIEvent(MIDI::ChannelOn1, MIDI::D4, MIDI::MaxVelocity));
		AddEvent(Key::D,
			MIDIEvent(MIDI::ChannelOn1, MIDI::E4, MIDI::MaxVelocity));
		AddEvent(Key::F,
			MIDIEvent(MIDI::ChannelOn1, MIDI::F4, MIDI::MaxVelocity));
		AddEvent(Key::G,
			MIDIEvent(MIDI::ChannelOn1, MIDI::G4, MIDI::MaxVelocity));
		AddEvent(Key::H,
			MIDIEvent(MIDI::ChannelOn1, MIDI::A4, MIDI::MaxVelocity));
		AddEvent(Key::J,
			MIDIEvent(MIDI::ChannelOn1, MIDI::B4, MIDI::MaxVelocity));
		AddEvent(Key::K,
			MIDIEvent(MIDI::ChannelOn1, MIDI::C5, MIDI::MaxVelocity));

		// BLACK KEYS: 'WE TYU'
		AddEvent(Key::W,
			MIDIEvent(MIDI::ChannelOn1, MIDI::CSharp4, MIDI::MaxVelocity));
		AddEvent(Key::E,
			MIDIEvent(MIDI::ChannelOn1, MIDI::DSharp4, MIDI::MaxVelocity));
		AddEvent(Key::T,
			MIDIEvent(MIDI::ChannelOn1, MIDI::FSharp4, MIDI::MaxVelocity));
		AddEvent(Key::Y,
			MIDIEvent(MIDI::ChannelOn1, MIDI::GSharp4, MIDI::MaxVelocity));
		AddEvent(Key::U,
			MIDIEvent(MIDI::ChannelOn1, MIDI::ASharp4, MIDI::MaxVelocity));
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

		MIDIEvent mEvent = m_MIDIMap.at(event.GetKeyCode());
		mEvent.data1 += octave;
		mEvent.data2 = m_Params->noteVelocity;
		if (mEvent.data1 > MIDI::G9)
			mEvent.data1 = MIDI::G9;

		m_RenderInfo->PushMIDIEvent(mEvent);

		// Puts release event in release map for when key is released
		// This is so if the octave or other control changes the key will always
		// release the corresponding noteOff event
		MIDIEvent releaseEvent = mEvent;
		releaseEvent.status = SynthBase::GetChannelOff(mEvent.status);

		m_ReleaseMap[event.GetKeyCode()] = releaseEvent;

		return true;
	}

	bool SynthEventHandler::OnKeyReleased(KeyReleasedEvent& event)
	{
		if (m_ReleaseMap.find(event.GetKeyCode()) == m_ReleaseMap.end())
			return false;

		MIDIEvent mEvent = m_ReleaseMap.at(event.GetKeyCode());

		m_RenderInfo->PushMIDIEvent(mEvent);

		return true;
	}
}