#include "bbpch.h"

#include "LowPassFilterFx.h"
namespace BackBeat {

	LowPassFilterFx::LowPassFilterFx()
	{

	}

	LowPassFilterFx::~LowPassFilterFx()
	{

	}

	void LowPassFilterFx::Update(LowPassFilterFxParameters params)
	{
		m_Params = params;
		m_CutoffFreq = m_Params.cutoff;
		float g = tan(2 * SynthBase::Pi * m_CutoffFreq / (float)m_SampleRate * m_Gain / 2);
		m_G = g / (1 + g);
	}

	void LowPassFilterFx::Reset()
	{
		m_S = 0.0f;

		float g = tan(2 * SynthBase::Pi * m_CutoffFreq / (float)m_SampleRate * m_Gain / 2);
		m_G = g / (1 + g);
	}

	float LowPassFilterFx::ProcessSample(float sample)
	{
		if (!m_Params.isOn)
			return sample;

		float output = 0.0f;
		float x = 0.0f;
		float y = 0.0f;
		float v = 0.0f;

		x = sample;
		v = (x - m_S) * m_G;
		y = v + m_S;
		m_S = y + v;

		output = y;
		return output;
	}

}