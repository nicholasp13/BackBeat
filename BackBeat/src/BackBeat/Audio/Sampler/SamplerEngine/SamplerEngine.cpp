#include "bbpch.h"

#include "SamplerEngine.h"
namespace BackBeat {

	SamplerEngine::SamplerEngine(AudioProps props)
		:
		m_NumVoices(SamplerMaxVoices),
		m_Buffer(std::make_shared<float[]>(props.sampleRate)),
		m_OutputBufferPosition(0),
		m_Props(props)
	{
		InitParameters();
		InitVoices();
	}

	SamplerEngine::~SamplerEngine()
	{

	}

	void SamplerEngine::Stop()
	{

	}

	void SamplerEngine::Reset(unsigned int sampleRate)
	{
		for (unsigned int i = 0; i < m_NumVoices; i++)
			m_Voices[i]->Reset(sampleRate);
	}

	void SamplerEngine::Render(std::shared_ptr<RenderInfo> info)
	{
		unsigned int numSamples = info->GetSamplesToRender();
		unsigned int bufferSize = info->GetBufferSize();
		std::shared_ptr<float[]> outputBuffer = info->GetBuffer();

		Audio::FlushBuffer(m_Buffer, numSamples, Audio::Stereo, 0.0f);

		while (!info->MIDIEventsEmpty())
		{
			ProcessMIDIEvent(info->PopMIDIEvent());
		}

		for (unsigned int i = 0; i < m_NumVoices; i++) {
			if (m_Voices[i]->IsActive())
			{
				m_Voices[i]->Render(numSamples);
			}
		}

		for (unsigned int j = 0; j < numSamples * Audio::Stereo; j++) {
			outputBuffer[j] = m_Buffer[j] * m_VoiceFactor;
			m_OutputBufferPosition = (m_OutputBufferPosition + 1) % bufferSize;
		}
	}

	void SamplerEngine::ProcessMIDIEvent(MIDIEvent event)
	{
		if (!Audio::IsNoteOn(event))
			return;

		for (unsigned int i = 0; i < m_NumVoices; i++) {
			if (event.data1 == m_Voices[i]->GetMIDINote())
			{
				m_Voices[i]->ProcessMIDIEvent(event);
				return;
			}
		}
	}

	void SamplerEngine::InitVoices()
	{
		// Offset pan
		m_VoiceFactor = 2.0f;

		for (unsigned int i = 0; i < m_NumVoices; i++)
			m_Voices[i] = std::make_unique<SamplerVoice>(m_Props.sampleRate, m_Buffer, m_Params[i]);
	}

	void SamplerEngine::InitParameters()
	{
		for (unsigned int i = 0; i < m_NumVoices; i++)
		{
			auto DCAParams = std::make_shared<DCAParameters>();
			DCAParams->leftAmp = 1.0f;
			DCAParams->rightAmp = 1.0f;
			DCAParams->volume = 1.0f;
			m_Params.push_back(DCAParams);
		}
	}
}