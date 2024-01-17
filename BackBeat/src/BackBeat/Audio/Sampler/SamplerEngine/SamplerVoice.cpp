#include "bbpch.h"

#include "SamplerVoice.h"
namespace BackBeat {

	SamplerVoice::SamplerVoice(unsigned int bufferSize, std::shared_ptr<float[]> outputBuffer, std::shared_ptr<DCAParameters> params)
		: 
		m_BufferSize(bufferSize), 
		m_MIDINote(MIDI::NoteOff), 
		m_InputBuffer(std::make_shared<float[]>(bufferSize)),
		m_OutputBuffer(outputBuffer),
		m_Params(params)
	{
		InitModules();
	}

	SamplerVoice::~SamplerVoice()
	{

	}

	void SamplerVoice::Reset(unsigned int sampleRate)
	{
		m_SamplerModule->Reset(sampleRate);
		m_DCA->Reset(sampleRate);
	}

	void SamplerVoice::Update()
	{

	}

	void SamplerVoice::Render(unsigned int numSamples)
	{
		m_SamplerModule->Render(numSamples);
		m_DCA->Render(numSamples);

		for (unsigned int i = 0; i < numSamples * Audio::Stereo; i++) {
			m_OutputBuffer[i] += m_InputBuffer[i];
		}
	}

	void SamplerVoice::ProcessMIDIEvent(MIDIEvent event)
	{
		NoteEvent nEvent = SynthBase::ConvertEvent(event);

		if (nEvent.noteOn)
			DoNoteOn(nEvent);
		else
			DoNoteOff(nEvent);
	}

	void SamplerVoice::DoNoteOn(NoteEvent event)
	{
		m_SamplerModule->DoNoteOn(event);
		m_DCA->DoNoteOn(event);
	}

	void SamplerVoice::DoNoteOff(NoteEvent event)
	{
		m_SamplerModule->DoNoteOff(event);
		m_DCA->DoNoteOff(event);
	}

	void SamplerVoice::InitModules()
	{
		m_SamplerModule = std::make_unique<SamplerModule>(m_BufferSize, m_InputBuffer);
		m_DCA = std::make_unique<DCA>(m_BufferSize, m_InputBuffer, m_Params);
	}

}