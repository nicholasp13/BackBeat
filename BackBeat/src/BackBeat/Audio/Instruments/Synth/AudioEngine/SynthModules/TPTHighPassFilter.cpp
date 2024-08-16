#include "bbpch.h"

#include "TPTHighPassFilter.h"
namespace BackBeat {

	TPTHighPassFilter::TPTHighPassFilter(unsigned int sampleRate, unsigned int bufferSize, std::shared_ptr<float[]> buffer, std::shared_ptr<FilterParameters> params)
		: m_Core(std::make_shared<TPTHPFilterCore>(sampleRate, bufferSize, buffer, params))
	{

	}

	TPTHighPassFilter::~TPTHighPassFilter()
	{

	}

	void TPTHighPassFilter::Reset(unsigned int sampleRate)
	{
		m_Core->Reset(sampleRate);
	}

	void TPTHighPassFilter::Update()
	{

	}

	void TPTHighPassFilter::Render(unsigned int numSamples)
	{
		m_Core->Render(numSamples);
	}

	void TPTHighPassFilter::DoNoteOn(NoteEvent event)
	{
		m_Core->DoNoteOn(event);
	}

	void TPTHighPassFilter::DoNoteOff(NoteEvent event)
	{
		m_Core->DoNoteOff(event);
	}
}