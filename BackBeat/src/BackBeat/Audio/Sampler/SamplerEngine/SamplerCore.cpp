#include "bbpch.h"

#include "SamplerCore.h"
namespace BackBeat {

	SamplerCore::SamplerCore(unsigned int sampleRate, std::shared_ptr<float[]> buffer)
		: 
		m_Looping(false),
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
			if (!m_Sample->IsDone() && !m_Looping)
			{
				unsigned int bytesToRender = numSamples * m_Sample->GetProps().blockAlign;
				m_Sample->Render(reinterpret_cast<byte*>(m_Input), bytesToRender);
				for (unsigned int i = 0; i < numSamples * Audio::Stereo; i++)
					m_Output[i] += m_Input[i];
			}
			else if (m_Looping)
			{
				unsigned int totalBytesLeft = numSamples * m_Sample->GetProps().blockAlign;
				unsigned int bytesToRender = 0;
				unsigned int samplePosition = m_Sample->GetBytePosition();
				unsigned int sampleSize = m_Sample->GetByteSize();
				unsigned int samplesRendered = 0;
				unsigned int outputPosition = 0;
				while (totalBytesLeft > 0)
				{
					if (m_Sample->IsDone())
						m_Sample->Reset();
					samplePosition = m_Sample->GetBytePosition();
					bytesToRender = (samplePosition + totalBytesLeft) > sampleSize
						? (sampleSize - samplePosition) : totalBytesLeft;
					m_Sample->Render(reinterpret_cast<byte*>(m_Input), bytesToRender);
					samplesRendered = bytesToRender / m_Sample->GetProps().blockAlign * Audio::Stereo;
					for (unsigned int i = 0; i < samplesRendered; i++) {
						m_Output[outputPosition] += m_Input[i];
						outputPosition++;
					}
					totalBytesLeft -= bytesToRender;
				}
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

	// TODO: Check if this can change into an inline
	void SamplerCore::LoopOn()
	{
		m_Looping = true;
		if (m_Sample)
		{

		}
	}

	void SamplerCore::LoopOff()
	{
		m_Looping = false;
		if (m_Sample)
		{

		}
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