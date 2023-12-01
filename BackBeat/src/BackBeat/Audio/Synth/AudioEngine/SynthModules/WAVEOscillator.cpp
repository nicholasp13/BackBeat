#include "bbpch.h"

#include "WAVEOscillator.h"
namespace BackBeat {
	WaveOscillator::WaveOscillator(UINT32 sampleRate, std::shared_ptr<float[]> buffer, std::shared_ptr<OscParameters> params)
		:
		m_Core(std::make_shared<WaveOscCore>(sampleRate, buffer, params))
	{
	}

	WaveOscillator::~WaveOscillator()
	{

	}

	void WaveOscillator::Reset(UINT32 sampleRate)
	{
		m_Core->Reset(sampleRate);
	}

	void WaveOscillator::Update()
	{

	}

	void WaveOscillator::Render(UINT32 numSamples)
	{
		Update();
		m_Core->Render(numSamples);
	}

	void WaveOscillator::DoNoteOn(NoteEvent event)
	{
		m_Core->DoNoteOn(event);
	}

	void WaveOscillator::DoNoteOff(NoteEvent event)
	{
		m_Core->DoNoteOff(event);
	}

}