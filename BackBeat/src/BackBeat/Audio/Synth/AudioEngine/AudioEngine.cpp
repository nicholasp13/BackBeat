#include "bbpch.h"

#include "AudioEngine.h"
namespace BackBeat {

	// TODO: Finely tune volume

	AudioEngine::AudioEngine(AudioProps props)
		: 
		m_NumVoices(SynthMaxVoices),
		m_Buffer(std::make_shared<float[]>(props.sampleRate)), 
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
		MIDIEvent noteOff = {
			.status = 0x00,
			.data1 = 0x00,
			.data2 = 0x00,
		};

		for (unsigned int i = 0; i < m_NumVoices; i++)
				m_Voices[i]->ProcessMIDIEvent(noteOff);
	}

	void AudioEngine::Reset(unsigned int sampleRate)
	{
		for (unsigned int i = 0; i < m_NumVoices; i++)
			m_Voices[i]->Reset(sampleRate);
	}

	void AudioEngine::Render(std::shared_ptr<RenderInfo> info)
	{
		float volume = m_Params->volume;
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

		for (unsigned int j = 0; j < numSamples * 2; j++) {
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

	void AudioEngine::ProcessMIDIEvent(MIDIEvent event)
	{
		bool noteOn = Audio::IsNoteOn(event);

		for (unsigned int i = 0; i < m_NumVoices; i++) {
			if (event.data1 == m_Voices[i]->GetNote()) 
			{
				m_Voices[i]->ProcessMIDIEvent(event);
				return;
			}
		}

		if (noteOn) 
		{
			for (unsigned int j = 0; j < m_NumVoices; j++) {
				if (!m_Voices[j]->IsActive()) 
				{
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
		m_VoiceFactor = 2.0f / (float)m_NumVoices; // 2.0f to offset panning

		for (unsigned int i = 0; i < m_NumVoices; i++)
			m_Voices[i] = std::make_unique<SynthVoice>(m_Props.sampleRate, m_Buffer, m_Params->voiceParams);
	}
	 

	void AudioEngine::InitParameters() 
	{
		auto DCAParams = std::make_shared<DCAParameters>();
		DCAParams->leftAmp = 1.0f;
		DCAParams->rightAmp = 1.0f;
		DCAParams->volume = 1.0f;

		auto AmpEGParams = std::make_shared<EGParameters>();
		AmpEGParams->attackDuration = SynthBase::EG1AttackTimeDefault;
		AmpEGParams->decayDuration = SynthBase::EG1DecayTimeDefault;
		AmpEGParams->releaseDuration = SynthBase::EG1ReleaseTimeDefault;
		AmpEGParams->sustainValue = SynthBase::EG1SustainLevelDefault;
		
		auto EGParams = std::make_shared<EGParameters>();
		EGParams->attackDuration = SynthBase::EG1AttackTimeDefault;
		EGParams->decayDuration = SynthBase::EG1DecayTimeDefault;
		EGParams->releaseDuration = SynthBase::EG1ReleaseTimeDefault;
		EGParams->sustainValue = SynthBase::EG1SustainLevelDefault;
		
		auto LFOParams1 = std::make_shared<LFOParameters>();
		LFOParams1->amp = SynthBase::LFOAttentuationDefault;
		LFOParams1->hertz = SynthBase::LFOFrequencyDefault;
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
		LPFilterParams->cutoff = SynthBase::FilterCutoffMax;

		auto HPFilterParams = std::make_shared<FilterParameters>();
		HPFilterParams->isOn = false;
		HPFilterParams->cutoff = SynthBase::FilterCutoffMin;

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