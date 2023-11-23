#include "bbpch.h"

#include "TPTHPFilterCore.h"
namespace BackBeat {

	TPTHPFilterCore::TPTHPFilterCore(UINT32 sampleRate, UINT32 bufferSize, std::shared_ptr<float[]> buffer, std::shared_ptr<FilterParameters> params)
		:
		m_SampleRate(sampleRate),
		m_OutputBufferSize(bufferSize),
		m_OutputBuffer(buffer),
		m_Input(std::make_unique<Modulator>(bufferSize)),
		m_Gain(1.0f),
		m_CutoffFreq(FILTER_CUTOFF_MIN),
		m_Resonance(0.0f),
		m_Type(ModuleType::LowPassFilter),
		m_Params(params),
		m_IsOn(true),
		m_g(0.0f),
		m_GHp(0.0f),
		m_S(0.0f)
	{

	}

	TPTHPFilterCore::~TPTHPFilterCore()
	{

	}

	void TPTHPFilterCore::Reset(UINT32 sampleRate)
	{

	}

	void TPTHPFilterCore::Update()
	{
		m_IsOn = m_Params->isOn;
		// TODO:
		//	Evaluate prewarping vs not in a visualizier
		if (!Audio::EqualsFloat(m_CutoffFreq, m_Params->cutoff, DELTA_CENTS_HZ))
		{
			// No prewarping
			m_CutoffFreq = m_Params->cutoff / m_SampleRate;
			m_g = m_CutoffFreq * m_Gain / 2;
			m_GHp = 1 / (1 + m_g);
		}
	}

	void TPTHPFilterCore::Render(UINT32 numSamples)
	{
		Update();

		// NOTE: IsOn will be controlled by cutoff frequency later.
		if (m_IsOn) {
			float x = 0.0f;
			float y = 0.0f;
			float xs = 0.0f;

			for (UINT32 i = 0; i < numSamples * STEREO; i++) {
				if (i % 2 == 0) {
					x = m_OutputBuffer[i];
					xs = x - m_S;
					y = xs * m_GHp;
					m_S += (y * 2 * m_g);
				}
				m_OutputBuffer[i] = y;
			}
		}
	}

	void TPTHPFilterCore::DoNoteOn(noteEvent event)
	{
		float noteFreq = event.note / m_SampleRate;
		m_S = 0.0f;

		// No prewarping
		m_CutoffFreq = m_Params->cutoff / m_SampleRate;
		m_g = m_CutoffFreq * m_Gain / 2;
		m_GHp = 1 / (1 + m_g);

	}

	void TPTHPFilterCore::DoNoteOff(noteEvent event)
	{

	}
}