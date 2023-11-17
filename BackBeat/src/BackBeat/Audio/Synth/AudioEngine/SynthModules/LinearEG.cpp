#include "bbpch.h"

#include "LinearEG.h"
namespace BackBeat {

	LinearEG::LinearEG(UINT32 sampleRate, UINT32 bufferSize, std::shared_ptr<EGParameters> params)
		:
		m_Core(std::make_shared<LinearEGCore>(sampleRate, bufferSize, params))
	{

	}

	LinearEG::~LinearEG()
	{

	}

	void LinearEG::Reset(UINT32 sampleRate)
	{
		m_Core->Reset(sampleRate);
	}

	void LinearEG::Update()
	{
		
	}

	void LinearEG::Render(UINT32 numSamples)
	{
		Update();
		m_Core->Render(numSamples);
	}

	void LinearEG::DoNoteOn(noteEvent event)
	{
		m_Core->DoNoteOn(event);
	}

	void LinearEG::DoNoteOff(noteEvent event)
	{
		m_Core->DoNoteOff(event);
	}
}