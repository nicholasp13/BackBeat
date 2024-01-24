#include "bbpch.h"

#include "SampleProgrammer.h"
namespace BackBeat {

	SampleProgrammer::SampleProgrammer(AudioProps props)
		: m_NumSamplePads(SamplerMaxVoices), m_Props(props)
	{

	}

	SampleProgrammer::~SampleProgrammer()
	{

	}

	void SampleProgrammer::ProgramNote(unsigned int index, MIDICode newNote)
	{
		if (newNote > MIDI::G9)
			return;
		
		for (unsigned int i = 0; i < m_NumSamplePads; i++) {
			if (i == index)
				continue;
			if (newNote == m_SamplePads[i]->GetNote())
				return;
		}

		m_SamplePads[index]->SetNote(newNote);
	}

	void SampleProgrammer::ProgramSample(unsigned int index)
	{
		Sample* newSample = SampleBuilder::BuildSample(FileDialog::OpenFile("Sample Files (*.smpl)\0*.smpl\0"));
		if (newSample)
			m_SamplePads[index]->SetSample(newSample);
	}

	void SampleProgrammer::ProgramSample(unsigned int index, Sample* sample)
	{
		if (sample)
		{
			if (sample->GetProps().sampleRate == m_Props.sampleRate && sample->GetProps().blockAlign == m_Props.blockAlign)
			{
				m_SamplePads[index]->SetSample(sample);
				return;
			}
			else
			{
				delete sample;
				return;
			}
		}
	}

	void SampleProgrammer::ProgramSample(unsigned int index, std::string filePath)
	{
		Sample* newSample = SampleBuilder::BuildSample(filePath);
		if (newSample)
		{
			if (newSample->GetProps().sampleRate == m_Props.sampleRate && newSample->GetProps().blockAlign == m_Props.blockAlign)
			{
				m_SamplePads[index]->SetSample(newSample);
				return;
			}
			else
			{
				delete newSample;
				return;
			}
		}
	}

}