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

		Audio::FlushBuffer(m_Buffer, numSamples, STEREO, 0.0f);

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

		auto AmpEGParams = std::make_shared<EGParameters>();
		AmpEGParams->attackDuration = EG1_ATTACK_TIME_DEFAULT;
		AmpEGParams->decayDuration = EG1_DECAY_TIME_DEFAULT;
		AmpEGParams->releaseDuration = EG1_RELEASE_TIME_DEFAULT;
		AmpEGParams->sustainValue = EG1_SUSTAIN_LEVEL_DEFAULT;
		
		auto EGParams = std::make_shared<EGParameters>();
		EGParams->attackDuration = EG1_ATTACK_TIME_DEFAULT;
		EGParams->decayDuration = EG1_DECAY_TIME_DEFAULT;
		EGParams->releaseDuration = EG1_RELEASE_TIME_DEFAULT;
		EGParams->sustainValue = EG1_SUSTAIN_LEVEL_DEFAULT;
		
		auto LFOParams1 = std::make_shared<LFOParameters>();
		LFOParams1->amp = LFO_ATT_DEFAULT;
		LFOParams1->hertz = LFO_FREQ_DEFAULT;
		LFOParams1->wave = WaveType::Sin;

		auto OSCParams1 = std::make_shared<OscParameters>();
		OSCParams1->amp = 1.0f;
		OSCParams1->octave = 1.0f; 
		OSCParams1->wave = WaveType::SawtoothUp;

		auto OSCParams2 = std::make_shared<OscParameters>();
		OSCParams2->amp = 0.0f;
		OSCParams2->octave = 1.0f;
		OSCParams2->wave = WaveType::SawtoothUp;

		auto OSCParams3 = std::make_shared<OscParameters>();
		OSCParams3->amp = 0.0f;
		OSCParams3->octave = 1.0f;
		OSCParams3->wave = WaveType::SawtoothUp;

		auto OSCParams4 = std::make_shared<OscParameters>();
		OSCParams4->amp = 0.0f;
		OSCParams4->octave = 1.0f;
		OSCParams4->wave = WaveType::SawtoothUp;

		auto LPFilterParams = std::make_shared<FilterParameters>();
		LPFilterParams->isOn = false;
		LPFilterParams->cutoff = FILTER_CUTOFF_MAX;

		auto HPFilterParams = std::make_shared<FilterParameters>();
		HPFilterParams->isOn = false;
		HPFilterParams->cutoff = FILTER_CUTOFF_MIN;

		auto modMatrixParams = std::make_shared<ModMatrixParameters>();

		auto voiceParams = std::make_shared<VoiceParameters>();
		voiceParams->DCAParams = DCAParams;
		voiceParams->AmpEGParams = AmpEGParams;
		voiceParams->EGParams = EGParams;
		voiceParams->LFOParams1 = LFOParams1;
		voiceParams->OscParams1 = OSCParams1;
		voiceParams->OscParams2 = OSCParams2;
		voiceParams->OscParams3 = OSCParams3;
		voiceParams->OscParams4 = OSCParams4;
		voiceParams->LPFilterParams = LPFilterParams;
		voiceParams->HPFilterParams = HPFilterParams;
		voiceParams->ModMatrixParams = modMatrixParams;

		m_Params = std::make_shared<EngineParameters>();
		m_Params->volume = 1.0f;
		m_Params->voiceParams = voiceParams;
	}
}