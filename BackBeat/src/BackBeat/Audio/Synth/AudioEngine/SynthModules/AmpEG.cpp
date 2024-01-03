#include "bbpch.h"

#include "AmpEG.h"
namespace BackBeat {

	AmpEG::AmpEG(unsigned int sampleRate, unsigned int bufferSize, std::shared_ptr<float[]> DCABuffer, std::shared_ptr<EGParameters> params)
		:
		m_Core(std::make_shared<LinearEGCore>(sampleRate, bufferSize, params)),
		m_InputBuffer(m_Core->GetOutputBuffer()),
		m_OutputBuffer(DCABuffer)
	{

	}

	AmpEG::~AmpEG()
	{

	}

	void AmpEG::Reset(unsigned int sampleRate)
	{
		m_Core->Reset(sampleRate);
	}

	void AmpEG::Update()
	{

	}

	void AmpEG::Render(unsigned int numSamples)
	{
		Update();
		m_Core->Render(numSamples);

		// NOTE: This step will be implemented by the ModularMatrix for all other modulations
		unsigned int totalSamples = numSamples * Audio::Stereo;
		for (unsigned int i = 0; i < totalSamples; i++)
		{
			m_OutputBuffer[i] *= m_InputBuffer[i];
		}
	}

	void AmpEG::DoNoteOn(NoteEvent event)
	{
		m_Core->DoNoteOn(event);
	}

	void AmpEG::DoNoteOff(NoteEvent event)
	{
		m_Core->DoNoteOff(event);
	}
}