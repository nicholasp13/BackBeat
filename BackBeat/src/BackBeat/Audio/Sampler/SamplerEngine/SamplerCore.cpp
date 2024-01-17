#include "bbpch.h"

#include "SamplerCore.h"
namespace BackBeat {

	SamplerCore::SamplerCore(unsigned int sampleRate, std::shared_ptr<float[]> buffer)
		: 
		m_Sample(nullptr),
		m_SampleRate(sampleRate),
		m_Input(new float[sampleRate]), 
		m_Output(buffer)
	{

	}

	SamplerCore::~SamplerCore()
	{
		delete m_Sample;
		delete[] m_Input;
	}

	void SamplerCore::Reset(unsigned int sampleRate)
	{

	}

	void SamplerCore::Update()
	{

	}

	void SamplerCore::Render(unsigned int numSamples)
	{
		Update();

		if (m_Sample)
		{
			if (!m_Sample->IsDone())
			{
				unsigned int bytesToRender = numSamples * m_Sample->GetProps().blockAlign;
				m_Sample->Render((byte*)m_Input, bytesToRender);
				for (unsigned int i = 0; i < numSamples * Audio::Stereo; i++)
					m_Output[i] += m_Input[i];
			}
			else
			{
				return;
			}
		}
	}

	void SamplerCore::DoNoteOn(NoteEvent event)
	{
		if (m_Sample)
			m_Sample->Reset();
	}

	void SamplerCore::DoNoteOff(NoteEvent event)
	{
		m_Sample->Off();
	}

	bool SamplerCore::IsActive()
	{
		if (m_Sample)
			return m_Sample->IsActive();
		return false;
	}

	void SamplerCore::SetSample(Sample* newSample)
	{
		delete m_Sample;
		m_Sample = newSample;
		m_Sample->Off();
	}

	std::string SamplerCore::GetName()
	{
		if (m_Sample)
			m_Sample->GetName();
		return "";
	}
}