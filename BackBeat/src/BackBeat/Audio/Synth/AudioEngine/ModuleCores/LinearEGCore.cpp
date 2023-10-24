#include "bbpch.h"

#include "LinearEGCore.h"
namespace BackBeat {

	LinearEGCore::LinearEGCore(UINT32 sampleRate, UINT32 bufferSize, std::shared_ptr<float[]> outputBuffer)
		:
		m_InputPosition(0),
		m_OutputSize(bufferSize),
		m_SampleRate(sampleRate),
		m_Value(0),
		m_Scalar(1.0f),
		m_SustainValue(0.5f),
		m_AttackIncrement(0.0f),
		m_DecayDecrement(0.0f),
		m_ReleaseDecrement(0.0f),
		m_AttackDuration(0.5f),
		m_DecayDuration(0.5f),
		m_ReleaseDuration(0.5f),
		m_State(EGState::Off),
		m_OutputBuffer(outputBuffer)
	{

	}

	LinearEGCore::~LinearEGCore()
	{

	}

	void LinearEGCore::Reset(UINT32 sampleRate)
	{

	}

	void LinearEGCore::Update()
	{

	}

	void LinearEGCore::Render(UINT32 numSamples)
	{
		Update();

		UINT32 totalSamples = numSamples * STEREO;

		for (UINT32 i = 0; i < totalSamples; i+= STEREO)
		{
			for (UINT32 j = 0; j < STEREO; j++)
			{
				if (j == 0) {
					switch (m_State)
					{

					case EGState::Attack:
					{
						m_Value += m_AttackIncrement;
						if (m_Value >= 1.0f)
						{
							m_Value = 1.0f;
							m_State = EGState::Decay;
						}
						break;
					}

					case EGState::Decay:
					{
						m_Value -= m_DecayDecrement;
						if (m_Value <= m_SustainValue)
						{
							m_Value = m_SustainValue;
							m_State = EGState::Sustain;
						}
						break;
					}

					case EGState::Sustain:
					{
						m_Value = m_SustainValue;
						break;
					}

					case EGState::Release:
					{
						m_Value -= m_ReleaseDecrement;
						if (m_Value <= 0)
						{
							m_Value = 0;
							m_State = EGState::Off;
						}
						break;
					}

					}
				}
				m_OutputBuffer[i + j] *= (m_Value * m_Scalar);
			}
		}
	}

	void LinearEGCore::DoNoteOn(noteEvent event) 
	{
		m_AttackIncrement = 1.0f / m_SampleRate / m_AttackDuration;
		m_DecayDecrement = (1.0f - m_SustainValue) / m_SampleRate / m_DecayDuration;
		m_ReleaseDecrement = m_SustainValue / m_SampleRate / m_ReleaseDuration;
		m_Value = -1.0f * m_AttackIncrement;
		m_Scalar = event.velocity;
		m_State = EGState::Attack;
	}
	
	void LinearEGCore::DoNoteOff(noteEvent event)
	{
		m_State = EGState::Release;
	}
}