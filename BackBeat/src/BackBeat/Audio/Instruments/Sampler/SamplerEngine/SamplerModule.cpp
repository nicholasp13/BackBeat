#include "bbpch.h"

#include "SamplerModule.h"
namespace BackBeat {

	SamplerModule::SamplerModule(unsigned int sampleRate, std::shared_ptr<float[]> buffer)
		: m_Core(std::make_shared<SamplerCore>(sampleRate, buffer))
	{

	}

	SamplerModule::~SamplerModule()
	{

	}

	void SamplerModule::Reset(unsigned int sampleRate)
	{
		m_Core->Reset(sampleRate);
	}

	void SamplerModule::Update()
	{
		m_Core->Update();
	}

	void SamplerModule::Render(unsigned int numSamples)
	{
		m_Core->Render(numSamples);
	}

	void SamplerModule::DoNoteOn(NoteEvent event)
	{
		m_Core->DoNoteOn(event);
	}

	void SamplerModule::DoNoteOff(NoteEvent event)
	{
		m_Core->DoNoteOff(event);
	}

}