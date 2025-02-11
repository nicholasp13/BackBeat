#include "bbpch.h"

#include "SynthEngine.h"
namespace BackBeat {

	SynthEngine::SynthEngine(AudioProps props)
		: 
		m_NumVoices(SynthMaxVoices),
		m_Buffer(std::make_shared<float[]>(props.sampleRate)), 
		m_Props(props)
	{
		InitParameters();
		InitVoices();
	}

	SynthEngine::~SynthEngine()
	{

	}

	void SynthEngine::Stop()
	{
		MIDIEvent noteOff = {
			.status = 0x00,
			.data1 = 0x00,
			.data2 = 0x00,
		};

		for (unsigned int i = 0; i < m_NumVoices; i++)
				m_Voices[i]->ProcessMIDIEvent(noteOff);
	}

	void SynthEngine::Reset(unsigned int sampleRate)
	{
		for (unsigned int i = 0; i < m_NumVoices; i++)
			m_Voices[i]->Reset(sampleRate);
	}

	void SynthEngine::Render(std::shared_ptr<RenderInfo> info)
	{
		float volume = m_Params->volume;
		unsigned int numSamples = info->GetSamplesToRender();
		unsigned int bufferSize = info->GetBufferSize();
		std::shared_ptr<float[]> outputBuffer = info->GetBuffer();

#if 1
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
		}
#else
		// LOGGING/TIMING

		BB_CORE_INFO("-----------------------------");

		Timer timer;
		float time = 0.0f;
		float lastTimeFrame = 0.0f;
		float deltaTime = 0.0f;

		timer.Start();

		Audio::FlushBuffer(m_Buffer, numSamples, Audio::Stereo, 0.0f);
		time = timer.GetTimeNano();
		deltaTime = time - lastTimeFrame;
		BB_CORE_INFO("FLUSH TIME (in nanoseconds):     {0}", deltaTime);
		lastTimeFrame = time;


		while (!info->MIDIEventsEmpty())
		{
			ProcessMIDIEvent(info->PopMIDIEvent());
		}
		time = timer.GetTimeNano();
		deltaTime = time - lastTimeFrame;
		BB_CORE_INFO("MIDI TIME (in nanoseconds):      {0}", deltaTime);
		lastTimeFrame = time;

		for (unsigned int i = 0; i < m_NumVoices; i++) {
			if (m_Voices[i]->IsActive())
			{
				m_Voices[i]->Render(numSamples);
			}
		}
		time = timer.GetTimeNano();
		deltaTime = time - lastTimeFrame;
		BB_CORE_INFO("VOICES TIME (in nanoseconds):    {0}", deltaTime);
		lastTimeFrame = time;

		for (unsigned int j = 0; j < numSamples * 2; j++) {
			outputBuffer[j] = m_Buffer[j] * m_VoiceFactor * volume;
		}
		time = timer.GetTimeNano();
		deltaTime = time - lastTimeFrame;
		BB_CORE_INFO("COPY TIME (in nanoseconds):      {0}", deltaTime);
		lastTimeFrame = time;

		BB_CORE_INFO("-----------------------------");
#endif

	}
	
	// Calls Voices update function
	// To implement if needed
	void SynthEngine::SetParam()
	{


	}

	std::shared_ptr<EngineParameters> SynthEngine::GetParam()
	{
		return m_Params;
	}

	void SynthEngine::ProcessMIDIEvent(MIDIEvent event)
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
		
		// Add switch cases for other status bytes as needed
	}
	
	// Custom for each engine. Current engine for a basic synth piano
	void SynthEngine::InitVoices()
	{
		m_VoiceFactor = 4.0f / (float)m_NumVoices; // 4.0f to offset panning

		for (unsigned int i = 0; i < m_NumVoices; i++)
			m_Voices[i] = std::make_unique<SynthVoice>(m_Props.sampleRate, m_Buffer, m_Params->voiceParams);
	}
	 

	void SynthEngine::InitParameters() 
	{
		auto DCAParams = std::make_shared<DCAParameters>();
		DCAParams->pan = SynthBase::PanDefault;
		DCAParams->volume = 1.0f;

		auto AmpEGParams = std::make_shared<EGParameters>();
		AmpEGParams->tracking = true;
		AmpEGParams->attackDuration = SynthBase::EG1AttackTimeDefault;
		AmpEGParams->decayDuration = SynthBase::EG1DecayTimeDefault;
		AmpEGParams->releaseDuration = SynthBase::EG1ReleaseTimeDefault;
		AmpEGParams->sustainValue = SynthBase::EG1SustainLevelDefault;
		
		auto EGParams = std::make_shared<EGParameters>();
		EGParams->tracking = true;
		EGParams->attackDuration = SynthBase::EG1AttackTimeDefault;
		EGParams->decayDuration = SynthBase::EG1DecayTimeDefault;
		EGParams->releaseDuration = SynthBase::EG1ReleaseTimeDefault;
		EGParams->sustainValue = SynthBase::EG1SustainLevelDefault;
		
		auto LFOParams1 = std::make_shared<LFOParameters>();
		LFOParams1->amp = SynthBase::LFOAttentuationDefault;
		LFOParams1->hertz = SynthBase::LFOFrequencyDefault;
		LFOParams1->delay = SynthBase::LFODelayMin;
		LFOParams1->wave = WaveType::Sin;

		auto OSCParams1 = std::make_shared<OscParameters>();
		OSCParams1->amp = 1.0f;
		OSCParams1->octave = SynthBase::WaveOctaveDefault;
		OSCParams1->dutyCycle = SynthBase::WaveDutyCyleDefault;
		OSCParams1->detune = SynthBase::WaveDetuneDefault;
		OSCParams1->wave = WaveType::SawtoothUp;

		auto OSCParams2 = std::make_shared<OscParameters>();
		OSCParams2->amp = 0.0f;
		OSCParams2->octave = SynthBase::WaveOctaveDefault;
		OSCParams2->dutyCycle = SynthBase::WaveDutyCyleDefault;
		OSCParams2->detune = SynthBase::WaveDetuneDefault;
		OSCParams2->wave = WaveType::SawtoothUp;

		auto OSCParams3 = std::make_shared<OscParameters>();
		OSCParams3->amp = 0.0f;
		OSCParams3->octave = SynthBase::WaveOctaveDefault;
		OSCParams3->dutyCycle = SynthBase::WaveDutyCyleDefault;
		OSCParams3->detune = SynthBase::WaveDetuneDefault;
		OSCParams3->wave = WaveType::SawtoothUp;

		auto OSCParams4 = std::make_shared<OscParameters>();
		OSCParams4->amp = 0.0f;
		OSCParams4->octave = SynthBase::WaveOctaveDefault;
		OSCParams4->dutyCycle = SynthBase::WaveDutyCyleDefault;
		OSCParams4->detune = SynthBase::WaveDetuneDefault;
		OSCParams4->wave = WaveType::SawtoothUp;

		auto LPLadderFilterParams = std::make_shared<LadderFilterParameters>();
		LPLadderFilterParams->isOn = false;
		LPLadderFilterParams->enableAutoLimiter = false;
		LPLadderFilterParams->cutoff = SynthBase::FilterCutoffMax;
		LPLadderFilterParams->Q = SynthBase::LadderFilterQDefault;
		LPLadderFilterParams->bassBoostPercent = SynthBase::LadderFilterBassBoostDefault;

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
		voiceParams->LPLadderFilterParams = LPLadderFilterParams;
		voiceParams->HPFilterParams = HPFilterParams;
		voiceParams->ModMatrixParams = modMatrixParams;

		m_Params = std::make_shared<EngineParameters>();
		m_Params->volume = 1.0f;
		m_Params->voiceParams = voiceParams;
	}
}