#pragma once

#include "BackBeat/Audio/Instruments/Sampler/SampleBuilder.h"
#include "SamplerPad.h"
namespace BackBeat {

	class SampleProgrammer
	{
	public:
		SampleProgrammer(AudioProps props);
		~SampleProgrammer();

		void ProgramNote(unsigned int index, MIDICode newNote);
		void ProgramSample(unsigned int index);
		void ProgramSample(unsigned int index, std::shared_ptr<Sample> sample);
		void ProgramSample(unsigned int index, std::string filePath);

		inline void AddSamplerPad(std::shared_ptr<SamplerPad> newPad) { m_SamplePads.push_back(newPad); }
		inline void ClearNote(unsigned int index) { m_SamplePads[index]->ClearNote(); }
		inline void ClearSample(unsigned int index) { m_SamplePads[index]->ClearSample(); }
		inline void LoopOn(unsigned int index) { m_SamplePads[index]->LoopOn(); }
		inline void LoopOff(unsigned int index) { m_SamplePads[index]->LoopOff(); }
		inline bool IsLooping(unsigned int index) { return m_SamplePads[index]->IsLooping(); }
		inline MIDICode GetPadNote(unsigned int index) { return m_SamplePads[index]->GetNote(); }
		inline std::string GetPadName(unsigned int index) { return m_SamplePads[index]->GetName(); }
		inline unsigned int GetNumPads() { return m_NumSamplePads; }
		inline AudioProps GetProps() { return m_Props; }
		inline std::shared_ptr<SamplerPad> GetSamplePad(unsigned int index) { return m_SamplePads[index]; }

	private:
		unsigned int m_NumSamplePads;
		AudioProps m_Props;
		std::vector< std::shared_ptr<SamplerPad> > m_SamplePads;

	};

}
