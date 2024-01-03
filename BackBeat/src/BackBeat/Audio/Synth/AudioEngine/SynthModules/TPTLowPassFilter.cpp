#include "bbpch.h"

#include "TPTLowPassFilter.h"
namespace BackBeat {

	TPTLowPassFilter::TPTLowPassFilter(unsigned int sampleRate, unsigned int bufferSize, std::shared_ptr<float[]> buffer, std::shared_ptr<FilterParameters> params)
		: m_Core(std::make_shared<TPTLPFilterCore>(sampleRate, bufferSize, buffer, params))
	{

	}

	TPTLowPassFilter::~TPTLowPassFilter()
	{

	}

	void TPTLowPassFilter::Reset(unsigned int sampleRate)
	{
		m_Core->Reset(sampleRate);
	}

	void TPTLowPassFilter::Update()
	{

	}

	void TPTLowPassFilter::Render(unsigned int numSamples)
	{
		m_Core->Render(numSamples);
	}

	void TPTLowPassFilter::DoNoteOn(NoteEvent event)
	{
		m_Core->DoNoteOn(event);
	}

	void TPTLowPassFilter::DoNoteOff(NoteEvent event)
	{
		m_Core->DoNoteOff(event);
	}
}