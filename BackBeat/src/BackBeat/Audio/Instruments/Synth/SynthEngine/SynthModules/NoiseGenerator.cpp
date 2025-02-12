#include "bbpch.h"

#include "NoiseGenerator.h"
namespace BackBeat {

	NoiseGenerator::NoiseGenerator(
		unsigned int sampleRate,
		unsigned int bufferSize,
		std::shared_ptr<float[]> outputBuffer,
		std::shared_ptr<NoiseGeneratorParameters> params)
		:
		m_Core(std::make_shared<NoiseGeneratorCore>(sampleRate, bufferSize, outputBuffer, params))
	{

	}
	
	
	NoiseGenerator::~NoiseGenerator()
	{

	}

	void NoiseGenerator::Reset(unsigned int sampleRate)
	{
		m_Core->Reset(sampleRate);
	}

	void NoiseGenerator::Update()
	{

	}

	void NoiseGenerator::Render(unsigned int numSamples)
	{
		Update();
		m_Core->Render(numSamples);
	}

	void NoiseGenerator::DoNoteOn(NoteEvent event)
	{
		m_Core->DoNoteOn(event);
	}

	void NoiseGenerator::DoNoteOff(NoteEvent event)
	{
		m_Core->DoNoteOn(event);
	}


}