#include "bbpch.h"

#include "LowFrequencyOscillator.h"
namespace BackBeat {

	LowFrequencyOscillator::LowFrequencyOscillator(UINT32 sampleRate, UINT32 bufferSize, std::shared_ptr<float[]> outputBuffer, std::shared_ptr<LFOParameters> params)
	{
		m_Core = std::make_shared<LFOCore>(sampleRate, bufferSize, params);
		m_InputBuffer = m_Core->GetOutputBuffer();
		m_OutputBuffer = outputBuffer;
	}
	
	LowFrequencyOscillator::~LowFrequencyOscillator()
	{

	}

	void LowFrequencyOscillator::Reset(UINT32 sampleRate)
	{
		m_Core->Reset(sampleRate);
	}

	void LowFrequencyOscillator::Update()
	{

	}

	void LowFrequencyOscillator::Render(UINT32 numSamples)
	{
		Update();
		m_Core->Render(numSamples);

		// NOTE: This step will be implemented by the ModularMatrix for all modulations. This is only temporary
		UINT32 totalSamples = numSamples * STEREO;
		for (UINT32 i = 0; i < totalSamples; i++)
		{
			m_OutputBuffer[i] += m_InputBuffer[i];
		}
	}

	void LowFrequencyOscillator::DoNoteOn(noteEvent event)
	{
		m_Core->DoNoteOn(event);
	}

	void LowFrequencyOscillator::DoNoteOff(noteEvent event)
	{
		m_Core->DoNoteOff(event);
	}
}