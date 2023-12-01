#include "bbpch.h"

#include "SynthVoice.h"
namespace BackBeat {

	SynthVoice::SynthVoice(UINT32 sampleRate, std::shared_ptr<float[]> outputBuffer, std::shared_ptr<VoiceParameters> voiceParams)
		:
		m_SampleRate(sampleRate),
		m_Actice(false),
		m_Channel(0),
		m_NotePressed(NOTE_OFF),
		m_Params(voiceParams),
		m_InputBuffer(std::make_shared<float[]>(sampleRate)),
		m_OutputBuffer(outputBuffer)
	{
		InitModules();
	}

	SynthVoice::~SynthVoice()
	{

	}

	// NOTE: Not used in current implementation. Holdover from following SynthLabs architecture. Subject to change/deletion
	void SynthVoice::Reset(UINT32 sampleRate)
	{
		m_Osc1->Reset(sampleRate);
		m_Osc2->Reset(sampleRate);
		m_Osc3->Reset(sampleRate);
		m_Osc4->Reset(sampleRate);
		m_EG->Reset(sampleRate);
		m_DCA->Reset(sampleRate);
		m_LFO1->Reset(sampleRate);
		m_LPFilter->Reset(sampleRate);
		m_HPFilter->Reset(sampleRate);
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
		m_LPFilter->Render(numSamples);
		m_HPFilter->Render(numSamples);
		m_DCA->Render(numSamples);

		for (UINT32 i = 0; i < numSamples * STEREO; i++) {
			m_OutputBuffer[i] += (m_InputBuffer[i]);
		}

		if (m_AmpEG->GetState() == EGState::Off)
		{
			m_Actice = false;
			m_Channel = 0;
			m_NotePressed = NOTE_OFF;
		}
	}
	void SynthVoice::ProcessMIDIEvent(MIDIEvent event)
	{
		NoteEvent nEvent = SynthBase::ConvertEvent(event);

		if (nEvent.noteOn)
			DoNoteOn(nEvent);
		else
			DoNoteOff(nEvent);
	}

	// TODO: On and off channel decided by note
	void SynthVoice::DoNoteOn(NoteEvent event)
	{
		m_Actice = true;
		m_Channel = event.channel;
		m_NotePressed = event.midiNote;
		m_Osc1->DoNoteOn(event);
		m_Osc2->DoNoteOn(event);
		m_Osc3->DoNoteOn(event);
		m_Osc4->DoNoteOn(event);
		m_EG->DoNoteOn(event);
		m_AmpEG->DoNoteOn(event);
		m_DCA->DoNoteOn(event);
		m_LFO1->DoNoteOn(event);
		m_LPFilter->DoNoteOn(event);
		m_HPFilter->DoNoteOn(event);
	}

	// NOTE: Only EGs have non-empty DoNoteOff() functions. Voice only calls other SynthModules
	//       to match function call hierarchy
	void SynthVoice::DoNoteOff(NoteEvent event)
	{
		m_Osc1->DoNoteOff(event);
		m_Osc2->DoNoteOff(event);
		m_Osc3->DoNoteOff(event);
		m_Osc4->DoNoteOff(event);
		m_EG->DoNoteOff(event);
		m_AmpEG->DoNoteOff(event);
		m_DCA->DoNoteOff(event);
		m_LFO1->DoNoteOff(event);
		m_LPFilter->DoNoteOff(event);
		m_HPFilter->DoNoteOff(event);
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
		m_LPFilter = std::make_unique<TPTLowPassFilter>(m_SampleRate, m_SampleRate, m_InputBuffer, m_Params->LPFilterParams);
		m_HPFilter = std::make_unique<TPTHighPassFilter>(m_SampleRate, m_SampleRate, m_InputBuffer, m_Params->HPFilterParams);
	}
}