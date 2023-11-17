#include "bbpch.h"

#include "SynthVoice.h"
namespace BackBeat {

	SynthVoice::SynthVoice(UINT32 sampleRate, std::shared_ptr<float[]> outputBuffer, std::shared_ptr<VoiceParameters> voiceParams)
		:
		m_SampleRate(sampleRate),
		m_Actice(false),
		m_Channel(0),
		m_Params(voiceParams),
		m_InputBuffer(std::make_shared<float[]>(sampleRate)),
		m_OutputBuffer(outputBuffer)
	{
		InitModules();
	}

	SynthVoice::~SynthVoice()
	{

	}

	void SynthVoice::Reset(UINT32 sampleRate)
	{
		m_Osc1->Reset(sampleRate);
		m_Osc2->Reset(sampleRate);
		m_Osc3->Reset(sampleRate);
		m_Osc4->Reset(sampleRate);
		m_EG->Reset(sampleRate);
		m_DCA->Reset(sampleRate);
	}

	// TODO: Implement after creating modulation matrix
	void SynthVoice::Update()
	{

	}

	void SynthVoice::Render(UINT32 numSamples)
	{
		// Render ModSources
		m_EG->Render(numSamples);
		m_AmpEG->Render(numSamples);
		m_LFO1->Render(numSamples);

		// TODO: Run ModMatrix here

		// Render ModDestinations
		m_Osc1->Render(numSamples);
		m_Osc2->Render(numSamples);
		m_Osc3->Render(numSamples);
		m_Osc4->Render(numSamples);
		m_DCA->Render(numSamples);

		for (UINT32 i = 0; i < numSamples * STEREO; i++) {
			m_OutputBuffer[i] += (m_InputBuffer[i]);
		}

		if (m_AmpEG->GetState() == EGState::Off)
		{
			m_Actice = false;
			m_Channel = 0;
		}
	}
	void SynthVoice::ProcessMIDIEvent(midiEvent event)
	{
		noteEvent nEvent = SynthBase::ConvertEvent(event);

		if (nEvent.noteOn)
			DoNoteOn(nEvent);
		else
			DoNoteOff(nEvent);
	}

	void SynthVoice::DoNoteOn(noteEvent event)
	{
		m_Actice = true;
		m_Channel = event.channel;
		m_Osc1->DoNoteOn(event);
		m_Osc2->DoNoteOn(event);
		m_Osc3->DoNoteOn(event);
		m_Osc4->DoNoteOn(event);
		m_EG->DoNoteOn(event);
		m_AmpEG->DoNoteOn(event);
		m_DCA->DoNoteOn(event);
		m_LFO1->DoNoteOn(event);
	}

	void SynthVoice::DoNoteOff(noteEvent event)
	{
		m_Osc1->DoNoteOff(event);
		m_Osc2->DoNoteOff(event);
		m_Osc3->DoNoteOff(event);
		m_Osc4->DoNoteOff(event);
		m_EG->DoNoteOff(event);
		m_AmpEG->DoNoteOff(event);
		m_DCA->DoNoteOff(event);
		m_LFO1->DoNoteOff(event);
	}

	void SynthVoice::InitModules()
	{
		m_DCA = std::make_unique<DCA>(m_SampleRate, m_InputBuffer, m_Params->DCAParams);
		m_Osc1 = std::make_unique<WaveOscillator>(m_SampleRate, m_InputBuffer, m_Params->OscParams1);
		m_Osc2 = std::make_unique<WaveOscillator>(m_SampleRate, m_InputBuffer, m_Params->OscParams2);
		m_Osc3 = std::make_unique<WaveOscillator>(m_SampleRate, m_InputBuffer, m_Params->OscParams3);
		m_Osc4 = std::make_unique<WaveOscillator>(m_SampleRate, m_InputBuffer, m_Params->OscParams4);
		m_EG = std::make_unique<LinearEG>(m_SampleRate, m_SampleRate, m_Params->EGParams);
		m_AmpEG = std::make_unique<AmpEG>(m_SampleRate, m_SampleRate, m_DCA->GetInputBuffer(), m_Params->AmpEGParams);
		m_LFO1 = std::make_unique<LowFrequencyOscillator>(m_SampleRate, m_SampleRate, m_Osc1->GetModInputBuffer(), m_Params->LFOParams1);
	}
}