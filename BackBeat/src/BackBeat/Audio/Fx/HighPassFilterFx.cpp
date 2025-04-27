#include "bbpch.h"

#include "HighPassFilterFx.h"
namespace BackBeat {

	HighPassFilterFx::HighPassFilterFx()
	{

	}

	HighPassFilterFx::~HighPassFilterFx()
	{

	}

	void HighPassFilterFx::Update(HighPassFilterFxParameters params)
	{
		m_Params = params;
		m_CutoffFreq = m_Params.cutoff;
		m_g = tan(2 * SynthBase::Pi * m_CutoffFreq / (float)m_SampleRate * m_Gain / 2);
		m_GHp =  1 / (1 + m_g);
	}

	void HighPassFilterFx::Reset()
	{
		m_S = 0.0f;

		m_g = tan(2 * SynthBase::Pi * m_CutoffFreq / (float)m_SampleRate * m_Gain / 2);
		m_GHp = 1 / (1 + m_g);
	}

	float HighPassFilterFx::ProcessSample(float sample)
	{
		if (!m_Params.isOn)
			return sample;

		float output = 0.0f;
		float x = 0.0f;
		float y = 0.0f;
		float xs = 0.0f;

		x = sample;
		xs = x - m_S;
		y = xs * m_GHp;
		m_S += (y * 2.0f * m_g);

		output = y;
		return output;
	}

}