#include "bbpch.h"

#include "NoiseGeneratorCore.h"
namespace BackBeat {

	NoiseGeneratorCore::NoiseGeneratorCore(
		unsigned int sampleRate, 
		unsigned int bufferSize, 
		std::shared_ptr<float[]> outputBuffer,
		std::shared_ptr<NoiseGeneratorParameters> params)
		:
		m_SampleRate(sampleRate),
		m_Level(SynthBase::NoiseGenDefault),
		m_InputBuffer(std::make_shared<float[]>(bufferSize)),
		m_OutputBuffer(outputBuffer),
		m_Params(params)
	{

	}

	NoiseGeneratorCore::~NoiseGeneratorCore()
	{

	}

	void NoiseGeneratorCore::Reset(unsigned int sampleRate)
	{

	}

	void NoiseGeneratorCore::Update()
	{
		m_On = m_Params->isOn;
		m_Level = m_Params->level;
	}

	void NoiseGeneratorCore::Render(unsigned int numSamples)
	{
		Update();

		if (m_Level <= 0.0f || !m_On)
			return;

		Audio::WhiteNoise(m_InputBuffer.get(), numSamples, m_Level);

		unsigned int outPosition = 0;
		for (unsigned int i = 0; i < numSamples; i++)
		{
			for (unsigned int j = 0; j < Audio::Stereo; j++)
			{
				m_OutputBuffer[outPosition] += m_InputBuffer.get()[i];
				outPosition++;
			}
		}
	}

	void NoiseGeneratorCore::DoNoteOn(NoteEvent event)
	{

	}

	void NoiseGeneratorCore::DoNoteOff(NoteEvent event)
	{

	}


}