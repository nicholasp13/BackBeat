#include "bbpch.h"

#include "WAVEOscillator.h"
namespace BackBeat {
	WAVEOscillator::WAVEOscillator(UINT32 sampleRate, std::shared_ptr<float[]> buffer, std::shared_ptr<OscParameters> params)
		:
		m_SampleRate(sampleRate),
		m_NumCores(0),
		m_Buffer(buffer)
	{
		InitModules(params);
	}

	WAVEOscillator::~WAVEOscillator()
	{

	}

	void WAVEOscillator::Reset(UINT32 sampleRate)
	{
		for (UINT32 i = 0; i < m_NumCores; i++)
			m_Cores[i]->Reset(sampleRate);
	}

	// TODO: Implement after creating ModulationMatrix object to allow for dynamic change
	void WAVEOscillator::Update()
	{

	}

	void WAVEOscillator::Render(UINT32 numSamples)
	{
		Update();

		for (UINT32 i = 0; i < m_NumCores; i++)
			m_Cores[i]->Render(numSamples);
	}

	void WAVEOscillator::DoNoteOn(noteEvent event)
	{
		for (UINT32 i = 0; i < m_NumCores; i++)
			m_Cores[i]->DoNoteOn(event);
	}

	void WAVEOscillator::DoNoteOff(noteEvent event)
	{
		for (UINT32 i = 0; i < m_NumCores; i++)
			m_Cores[i]->DoNoteOff(event);
	}

	void WAVEOscillator::InitModules(std::shared_ptr<OscParameters> params)
	{
		std::shared_ptr<Oscillator> waveOscillator = std::make_shared<Oscillator>(m_SampleRate, m_Buffer, params);
		AddModule(waveOscillator);
	}

	void WAVEOscillator::AddModule(std::shared_ptr<ModuleCore> module)
	{
		m_Cores.push_back(module);
		m_NumCores++;
	}

}