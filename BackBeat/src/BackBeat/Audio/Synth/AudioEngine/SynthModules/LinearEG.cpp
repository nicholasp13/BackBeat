#include "bbpch.h"

#include "LinearEG.h"
namespace BackBeat {

	LinearEG::LinearEG(unsigned int sampleRate, unsigned int bufferSize, std::shared_ptr<EGParameters> params)
		:
		m_Core(std::make_shared<LinearEGCore>(sampleRate, bufferSize, params))
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