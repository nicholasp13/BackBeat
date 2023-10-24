#include "bbpch.h"

#include "AudioEngine.h"
namespace BackBeat {

	AudioEngine::AudioEngine(tWAVEFORMATEX props)
		: 
		m_Buffer(std::make_shared<float[]>(props.nSamplesPerSec)), 
		m_OutputBufferPosition(0),
		m_Props(props)
	{
		InitVoices();
	}

	AudioEngine::~AudioEngine()
	{

	}

	void AudioEngine::Reset(UINT32 sampleRate)
	{
		for (UINT32 i = 0; i < m_NumVoices; i++)
			m_Voices[i]->Reset(sampleRate);
	}

	void AudioEngine::Render(std::shared_ptr<RenderInfo> info)
	{
		UINT32 numSamples = info->GetSamplesToRender();
		UINT32 bufferSize = info->GetBufferSize();
		std::shared_ptr<float[]> outputBuffer = info->GetBuffer();

		FlushBuffer(numSamples);

		while (!info->MIDIEventsEmpty()) {
			ProcessMIDIEvent(info->PopMIDIEvent());
		}

		for (UINT32 i = 0; i < m_NumVoices; i++) {
			if (m_Voices[i]->IsActive()) {
				m_Voices[i]->Render(numSamples);
			}
		}

		for (UINT32 j = 0; j < numSamples * 2; j++) {
			outputBuffer[j] = m_Buffer[j] * m_VoiceFactor;
			m_OutputBufferPosition = (m_OutputBufferPosition + 1) % bufferSize;
		}
	}
	
	// Calls Voices update function
	void AudioEngine::SetParam() // TODO: Implement
	{

	}

	void AudioEngine::GetParam() // TODO: Implement
	{

	}

	void AudioEngine::ProcessMIDIEvent(midiEvent event)
	{
		int channel = 0;
		bool noteOn = SynthBase::IsNoteOn(event);
		if (noteOn) {
			channel = SynthBase::GetMidiChannel(event, noteOn);
			for (UINT32 i = 0; i < m_NumVoices; i++) {
				if (!m_Voices[i]->IsActive() || m_Voices[i]->GetChannel() == channel) {
					m_Voices[i]->ProcessMIDIEvent(event);
					return;
				}
			}
		}

		channel = SynthBase::GetMidiChannel(event, noteOn);
		if (channel >= 0 && channel <= 16)
		{
			for (UINT32 j = 0; j < m_NumVoices; j++) {
				if (channel == m_Voices[j]->GetChannel()) {
					m_Voices[j]->ProcessMIDIEvent(event);
					return;
				}
			}
		}

		// TODO: Add switch cases for other status bytes
	}
	
	void AudioEngine::FlushBuffer(UINT32 numSamples)
	{
		for (UINT32 i = 0; i < numSamples * 2; i++)
			m_Buffer[i] = 0;
	}

	// Custom for each engine. Current basic synth engine
	void AudioEngine::InitVoices()
	{
		m_NumVoices = 13;
		m_VoiceFactor = 1.0f / (float)m_NumVoices;

		for (UINT32 i = 0; i < m_NumVoices; i++)
			m_Voices[i] = std::make_unique<SynthVoice>(m_Buffer, m_Props.nSamplesPerSec, m_Props.nSamplesPerSec);
	}
}