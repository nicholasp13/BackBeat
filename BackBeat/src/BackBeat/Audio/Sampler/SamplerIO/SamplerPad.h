#pragma once

#include "BackBeat/Audio/MIDI/MIDIInputHandler.h"
#include "BackBeat/Audio/Sampler/SamplerEngine/SamplerEngine.h"
namespace BackBeat {

	class SamplerPad
	{
	public:
		SamplerPad(std::shared_ptr<MIDIInputHandler> midiInputHandler,
			std::shared_ptr<SamplerEngine> samplerEngine, unsigned int index);
		~SamplerPad();

		void Press();
		void ClearNote();
		void ClearSample();
		void SetNote(MIDICode newNote);
		void SetSample(Sample* newSample);

		inline MIDICode GetNote() { return m_MIDINote; }
		inline std::string GetName() { return m_Name; }

	private:
		unsigned int m_Index;
		MIDICode m_MIDINote;
		std::string m_Name;
		std::shared_ptr<MIDIInputHandler> m_MIDIInputHandler;
		std::shared_ptr<SamplerEngine> m_Engine;
	};

}