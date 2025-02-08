#include "bbpch.h"

#include "LinearEG.h"
namespace BackBeat {

	LinearEG::LinearEG(unsigned int sampleRate, unsigned int bufferSize, std::shared_ptr<float[]> outputBuffer,
		std::shared_ptr<EGParameters> params)
		:
		m_Core(std::make_shared<LinearEGCore>(sampleRate, bufferSize, params)),
		m_InputBuffer(m_Core->GetOutputBuffer()),
		m_OutputBuffer(outputBuffer)
	{

	}

	LinearEG::~LinearEG()
	{

	}

	void LinearEG::Reset(unsigned int sampleRate)
	{
		m_Core->Reset(sampleRate);
	}

	void LinearEG::Update()
	{
		
	}

	void LinearEG::Render(unsigned int numSamples)
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

	void LinearEG::DoNoteOn(NoteEvent event)
	{
		m_Core->DoNoteOn(event);
	}

	void LinearEG::DoNoteOff(NoteEvent event)
	{
		m_Core->DoNoteOff(event);
	}
}