#include "bbpch.h"

#include "TPTLowPassFilter.h"
namespace BackBeat {

	TPTLowPassFilter::TPTLowPassFilter(UINT32 sampleRate, UINT32 bufferSize, std::shared_ptr<float[]> buffer, std::shared_ptr<FilterParameters> params)
		: m_Core(std::make_shared<TPTLPFilterCore>(sampleRate, bufferSize, buffer, params))
	{

	}

	TPTLowPassFilter::~TPTLowPassFilter()
	{

	}

	void TPTLowPassFilter::Reset(UINT32 sampleRate)
	{
		m_Core->Reset(sampleRate);
	}

	void TPTLowPassFilter::Update()
	{

	}

	void TPTLowPassFilter::Render(UINT32 numSamples)
	{
		m_Core->Render(numSamples);
	}

	void TPTLowPassFilter::DoNoteOn(noteEvent event)
	{
		m_Core->DoNoteOn(event);
	}

	void TPTLowPassFilter::DoNoteOff(noteEvent event)
	{
		m_Core->DoNoteOff(event);
	}
}