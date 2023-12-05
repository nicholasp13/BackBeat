#include "bbpch.h"

#include "AmpEG.h"
namespace BackBeat {

	AmpEG::AmpEG(UINT32 sampleRate, UINT32 bufferSize, std::shared_ptr<float[]> DCABuffer, std::shared_ptr<EGParameters> params)
		:
		m_Core(std::make_shared<LinearEGCore>(sampleRate, bufferSize, params)),
		m_InputBuffer(m_Core->GetOutputBuffer()),
		m_OutputBuffer(DCABuffer)
	{

	}

	AmpEG::~AmpEG()
	{

	}

	void AmpEG::Reset(UINT32 sampleRate)
	{
		m_Core->Reset(sampleRate);
	}

	void AmpEG::Update()
	{

	}

	void AmpEG::Render(UINT32 numSamples)
	{
		Update();
		m_Core->Render(numSamples);

		// NOTE: This step will be implemented by the ModularMatrix for all other modulations
		UINT32 totalSamples = numSamples * STEREO;
		for (UINT32 i = 0; i < totalSamples; i++)
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