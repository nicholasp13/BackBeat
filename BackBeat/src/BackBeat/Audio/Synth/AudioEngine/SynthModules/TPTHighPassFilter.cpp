#include "bbpch.h"

#include "TPTHighPassFilter.h"
namespace BackBeat {

	TPTHighPassFilter::TPTHighPassFilter(UINT32 sampleRate, UINT32 bufferSize, std::shared_ptr<float[]> buffer, std::shared_ptr<FilterParameters> params)
		: m_Core(std::make_shared<TPTHPFilterCore>(sampleRate, bufferSize, buffer, params))
	{

	}

	TPTHighPassFilter::~TPTHighPassFilter()
	{

	}

	void TPTHighPassFilter::Reset(UINT32 sampleRate)
	{
		m_Core->Reset(sampleRate);
	}

	void TPTHighPassFilter::Update()
	{

	}

	void TPTHighPassFilter::Render(UINT32 numSamples)
	{
		m_Core->Render(numSamples);
	}

	void TPTHighPassFilter::DoNoteOn(noteEvent event)
	{
		m_Core->DoNoteOn(event);
	}

	void TPTHighPassFilter::DoNoteOff(noteEvent event)
	{
		m_Core->DoNoteOff(event);
	}
}