#include "bbpch.h"

#include "LowFrequencyOscillator.h"
namespace BackBeat {

	LowFrequencyOscillator::LowFrequencyOscillator(unsigned int sampleRate, unsigned int bufferSize, std::shared_ptr<float[]> outputBuffer, std::shared_ptr<LFOParameters> params)
	{
		m_Core = std::make_shared<LFOCore>(sampleRate, bufferSize, params);
		m_InputBuffer = m_Core->GetOutputBuffer();
		m_OutputBuffer = outputBuffer;
	}
	
	LowFrequencyOscillator::~LowFrequencyOscillator()
	{

	}

	void LowFrequencyOscillator::Reset(unsigned int sampleRate)
	{
		m_Core->Reset(sampleRate);
	}

	void LowFrequencyOscillator::Update()
	{

	}

	void LowFrequencyOscillator::Render(unsigned int numSamples)
	{
		Update();
		m_Core->Render(numSamples);

		// NOTE: This step will be implemented by the ModularMatrix for all modulations. This is only temporary
		unsigned int totalSamples = numSamples * Audio::Stereo;
		for (unsigned int i = 0; i < totalSamples; i++)
		{
			m_OutputBuffer[i] += m_InputBuffer[i];
		}
	}

	void LowFrequencyOscillator::DoNoteOn(NoteEvent event)
	{
		m_Core->DoNoteOn(event);
	}

	void LowFrequencyOscillator::DoNoteOff(NoteEvent event)
	{
		m_Core->DoNoteOff(event);
	}
}