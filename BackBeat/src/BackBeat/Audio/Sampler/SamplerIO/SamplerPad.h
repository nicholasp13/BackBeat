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

		inline void LoopOn() { m_Engine->LoopOn(m_Index); }
		inline void LoopOff() { m_Engine->LoopOff(m_Index); }
		inline bool IsLooping() { return m_Engine->IsLooping(m_Index); }
		inline MIDICode GetNote() { return m_MIDINote; }
		inline std::string GetName() { return m_Name; }
		inline std::shared_ptr<DCAParameters> GetDCAParameters() { return m_DCAParameters; }

	private:
		unsigned int m_Index;
		MIDICode m_MIDINote;
		std::string m_Name;
		std::shared_ptr<MIDIInputHandler> m_MIDIInputHandler;
		std::shared_ptr<SamplerEngine> m_Engine;
		std::shared_ptr<DCAParameters> m_DCAParameters;
	};

}