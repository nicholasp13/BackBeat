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
		m_OutputBuffer(outputBuffer),
		m_DCA(std::make_unique<DCA>(m_InputBuffer, voiceParams->DCAParams) ),
		m_Oscillator(std::make_unique<WAVEOscillator>(sampleRate, m_InputBuffer, voiceParams->OscParams)),
		m_EG(std::make_unique<LinearEG>(sampleRate, m_InputBuffer, voiceParams->EGParams))
	{

	}

	SynthVoice::~SynthVoice()
	{

	}

	void SynthVoice::Reset(UINT32 sampleRate)
	{
		m_Oscillator->Reset(sampleRate);
		m_EG->Reset(sampleRate);
		m_DCA->Reset(sampleRate);
	}

	// TODO: Implement after creating modulation matrix
	void SynthVoice::Update()
	{

	}

	void SynthVoice::Render(UINT32 numSamples)
	{
		m_Oscillator->Render(numSamples);
		m_EG->Render(numSamples);
		m_DCA->Render(numSamples);

		for (UINT32 i = 0; i < numSamples * 2; i++) {
			m_OutputBuffer[i] += m_InputBuffer[i];
		}

		if (m_EG->GetState() == EGState::Off)
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
		m_Oscillator->DoNoteOn(event);
		m_EG->DoNoteOn(event);
		m_DCA->DoNoteOn(event);
	}

	void SynthVoice::DoNoteOff(noteEvent event)
	{
		m_Oscillator->DoNoteOff(event);
		m_EG->DoNoteOff(event);
		m_DCA->DoNoteOff(event);
	}

}