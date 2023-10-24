#include "bbpch.h"

#include "LinearEG.h"
namespace BackBeat {

	LinearEG::LinearEG(UINT32 sampleRate, UINT32 bufferSize, std::shared_ptr<float[]> buffer)
		:
		m_SampleRate(sampleRate),
		m_BufferSize(bufferSize),
		m_Buffer(buffer),
		m_Core(std::make_shared<LinearEGCore>(sampleRate, bufferSize, buffer))
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