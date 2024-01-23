#include "bbpch.h"

#include "SamplerPad.h"
namespace BackBeat {

	SamplerPad::SamplerPad(std::shared_ptr<MIDIInputHandler> midiInputHandler, 
		std::shared_ptr<SamplerEngine> samplerEngine, unsigned int index)
		:
		m_Index(index),
		m_MIDINote(MIDI::NoteOff),
		m_Name(""),
		m_MIDIInputHandler(midiInputHandler),
		m_Engine(samplerEngine)
	{
		m_DCAParameters = m_Engine->GetParam(index);
	}

	SamplerPad::~SamplerPad()
	{

	}

	void SamplerPad::Press()
	{
		if (m_MIDINote == MIDI::NoteOff)
			return;
		MIDIEvent midiEvent = MIDIEvent();
		// NOTE: Might want to change constants to private members to allow for programmability of those bytes from input/outside
		midiEvent.status = MIDI::ChannelOn1;
		midiEvent.data1 = m_MIDINote;
		midiEvent.data2 = MIDI::MaxVelocity;
		m_MIDIInputHandler->Push(midiEvent);
	}

	void SamplerPad::ClearNote()
	{
		m_MIDINote = MIDI::NoteOff;
		m_Engine->ClearMIDINote(m_Index);
	}

	void SamplerPad::ClearSample()
	{
		m_Name = "";
		m_Engine->ClearSample(m_Index);
	}

	void SamplerPad::SetNote(MIDICode newNote)
	{
		m_MIDINote = newNote;
		m_Engine->SetNote(newNote, m_Index);
	}

	void SamplerPad::SetSample(Sample* newSample)
	{
		m_Name = newSample->GetName();
		m_Engine->SetSample(newSample, m_Index);
	}

}