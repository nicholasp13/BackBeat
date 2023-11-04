#include "bbpch.h"

#include "AudioEngine.h"
namespace BackBeat {

	AudioEngine::AudioEngine(tWAVEFORMATEX props)
		: 
		m_Buffer(std::make_shared<float[]>(props.nSamplesPerSec)), 
		m_OutputBufferPosition(0),
		m_Props(props)
	{
		InitParameters();
		InitVoices();
	}

	AudioEngine::~AudioEngine()
	{

	}

	void AudioEngine::Stop()
	{
		midiEvent noteOff = {
			.status = 0x00,
			.data1 = 0x00,
			.data2 = 0x00,
		};

		for (UINT32 i = 0; i < m_NumVoices; i++)
				m_Voices[i]->ProcessMIDIEvent(noteOff);
	}

	void AudioEngine::Reset(UINT32 sampleRate)
	{
		for (UINT32 i = 0; i < m_NumVoices; i++)
			m_Voices[i]->Reset(sampleRate);
	}

	void AudioEngine::Render(std::shared_ptr<RenderInfo> info)
	{
		float volume = m_Params->volume;
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
			outputBuffer[j] = m_Buffer[j] * m_VoiceFactor * volume;
			m_OutputBufferPosition = (m_OutputBufferPosition + 1) % bufferSize;
		}
	}
	
	// Calls Voices update function
	void AudioEngine::SetParam() // TODO: Implement if needed
	{

	}

	// Subject to change
	std::shared_ptr<EngineParameters> AudioEngine::GetParam()
	{
		return m_Params;
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

	// Custom for each engine. Current engine for a basic synth piano
	void AudioEngine::InitVoices()
	{
		m_NumVoices = 13;
		m_VoiceFactor = 1.0f / (float)m_NumVoices;

		for (UINT32 i = 0; i < m_NumVoices; i++)
			m_Voices[i] = std::make_unique<SynthVoice>(m_Props.nSamplesPerSec, m_Buffer, m_Params->voiceParams);
	}
	 

	void AudioEngine::InitParameters() 
	{
		auto DCAParams = std::make_shared<DCAParameters>();
		DCAParams->leftAmp = 1.0f;
		DCAParams->rightAmp = 1.0f;
		
		auto EGParams = std::make_shared<EGParameters>();
		EGParams->attackDuration = EG1_ATTACK_TIME_DEFAULT;
		EGParams->decayDuration = EG1_DECAY_TIME_DEFAULT;
		EGParams->releaseDuration = EG1_RELEASE_TIME_DEFAULT;
		EGParams->sustainValue = EG1_SUSTAIN_LEVEL_DEFAULT;
		
		auto OSCParams = std::make_shared<OscParameters>();
		OSCParams->amp = 1.0f;
		OSCParams->wave = WaveType::SawtoothUp;
		
		auto modMatrixParams = std::make_shared<ModMatrixParameters>();

		auto voiceParams = std::make_shared<VoiceParameters>();
		voiceParams->DCAParams = DCAParams;
		voiceParams->EGParams = EGParams;
		voiceParams->OscParams = OSCParams;
		voiceParams->ModMatrixParams = modMatrixParams;

		m_Params = std::make_shared<EngineParameters>();
		m_Params->volume = 1.0f;
		m_Params->voiceParams = voiceParams;
	}
}