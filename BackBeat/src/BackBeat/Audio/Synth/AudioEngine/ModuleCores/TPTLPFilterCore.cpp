#include "bbpch.h"

#include "TPTLPFilterCore.h"
namespace BackBeat {

	TPTLPFilterCore::TPTLPFilterCore(UINT32 sampleRate, UINT32 bufferSize, std::shared_ptr<float[]> buffer, std::shared_ptr<FilterParameters> params)
		:
		m_SampleRate(sampleRate),
		m_OutputBufferSize(bufferSize),
		m_OutputBuffer(buffer),
		m_Input(std::make_unique<Modulator>(bufferSize)),
		m_Gain(1.0f),
		m_CutoffFreq(FILTER_CUTOFF_MAX),
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

	void TPTLPFilterCore::Reset(UINT32 sampleRate)
	{

	}

	void TPTLPFilterCore::Update()
	{
		m_IsOn = m_Params->isOn;
		// TODO: Evaluate prewarping vs not in a visualizier
		if (!Audio::EqualsFloat(m_CutoffFreq, m_Params->cutoff, DELTA_CENTS_HZ))
		{
			// No prewarping
			m_CutoffFreq = m_Params->cutoff / m_SampleRate;
			float g = m_CutoffFreq * m_Gain / 2;
			m_G = g / (1 + g);
		}
	}

	void TPTLPFilterCore::Render(UINT32 numSamples)
	{
		Update();

		// NOTE: Will be controlled by cutoff frequency later
		if (m_IsOn) {
			float x = 0.0f;
			float y = 0.0f;
			float v = 0.0f;

			for (UINT32 i = 0; i < numSamples * STEREO; i++) {
				if (i % 2 == 0) {
					x = m_OutputBuffer[i];
					v = (x - m_S) * m_G;
					y = v + m_S;
					m_S = y + v;
				}
				m_OutputBuffer[i] = y;
			}
		}
	}

	void TPTLPFilterCore::DoNoteOn(noteEvent event)
	{
		float noteFreq = event.note / m_SampleRate;
		m_S = 0.0f;

		// No prewarping
		m_CutoffFreq = m_Params->cutoff / m_SampleRate;
		float g = m_CutoffFreq * m_Gain / 2;
		m_G = g / (1 + g);

	}

	void TPTLPFilterCore::DoNoteOff(noteEvent event)
	{
		
	}
}