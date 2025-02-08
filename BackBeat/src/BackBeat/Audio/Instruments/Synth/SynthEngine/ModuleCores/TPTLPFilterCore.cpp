#include "bbpch.h"

#include "TPTLPFilterCore.h"
namespace BackBeat {

	TPTLPFilterCore::TPTLPFilterCore(unsigned int sampleRate, unsigned int bufferSize, std::shared_ptr<float[]> buffer, std::shared_ptr<FilterParameters> params)
		:
		m_SampleRate(sampleRate),
		m_OutputBufferSize(bufferSize),
		m_OutputBuffer(buffer),
		m_Input(std::make_unique<Modulator>(bufferSize)),
		m_Gain(1.0f),
		m_CutoffFreq(SynthBase::FilterCutoffMax),
		m_Resonance(0.0f),
		m_Type(ModuleType::LowPassFilter),
		m_Params(params),
		m_IsOn(true),
		m_G(0.0f),
		m_S(0.0f)
	{

	}
	
	TPTLPFilterCore::~TPTLPFilterCore()
	{

	}

	void TPTLPFilterCore::Reset(unsigned int sampleRate)
	{

	}

	void TPTLPFilterCore::Update()
	{
		m_IsOn = m_Params->isOn;

		if (!Audio::EqualsFloat(m_CutoffFreq, m_Params->cutoff, SynthBase::DeltaCentsHertz))
		{
			// No prewarping
			m_CutoffFreq = m_Params->cutoff / m_SampleRate;
			float g = m_CutoffFreq * m_Gain / 2;
			m_G = g / (1 + g);
		}
	}

	void TPTLPFilterCore::Render(unsigned int numSamples)
	{
		Update();

		// NOTE: Will be controlled by cutoff frequency later
		if (m_IsOn) 
		{
			float x = 0.0f;
			float y = 0.0f;
			float v = 0.0f;

			for (unsigned int i = 0; i < numSamples * Audio::Stereo; i++) {
				if (i % 2 == 0) 
				{
					x = m_OutputBuffer[i];
					v = (x - m_S) * m_G;
					y = v + m_S;
					m_S = y + v;
				}
				m_OutputBuffer[i] = y;
			}
		}
	}

	void TPTLPFilterCore::DoNoteOn(NoteEvent event)
	{
		m_S = 0.0f;

		// No prewarping
		m_CutoffFreq = m_Params->cutoff / m_SampleRate;
		float g = m_CutoffFreq * m_Gain / 2;
		m_G = g / (1 + g);

	}

	void TPTLPFilterCore::DoNoteOff(NoteEvent event)
	{
		
	}
}