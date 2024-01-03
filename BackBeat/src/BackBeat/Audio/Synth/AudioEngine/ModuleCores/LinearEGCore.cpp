#include "bbpch.h"

#include "LinearEGCore.h"
namespace BackBeat {

	LinearEGCore::LinearEGCore(unsigned int sampleRate, unsigned int bufferSize, std::shared_ptr<EGParameters> params)
		:
		m_SampleRate(sampleRate),
		m_InputPosition(0),
		m_Value(0),
		m_ScalarNote(0.0f),
		m_ScalarVelocity(0.0f),
		m_AttackIncrement(0.0f),
		m_DecayDecrement(0.0f),
		m_ReleaseDecrement(0.0f),
		m_SustainValue(params->sustainValue),
		m_State(EGState::Off),
		m_Params(params),
		m_Input(std::make_unique<Modulator>(bufferSize)),
		m_Output(std::make_unique<Modulator>(bufferSize))
	{

	}

	LinearEGCore::~LinearEGCore()
	{

	}

	void LinearEGCore::Reset(unsigned int sampleRate)
	{

	}

	void LinearEGCore::Update()
	{
		float attackDuration = m_Params->attackDuration * m_ScalarVelocity;
		float decayDuration = m_Params->decayDuration * m_ScalarNote;
		float releaseDuration = m_Params->releaseDuration;
		m_SustainValue = m_Params->sustainValue;
		if (attackDuration <= 0) 
		{
			m_AttackIncrement = 1.0f;
		}
		else 
		{
			m_AttackIncrement = 1.0f / m_SampleRate / attackDuration;
		}

		if (decayDuration <= 0) 
		{
			m_DecayDecrement = 1.0f - m_SustainValue;
		}
		else 
		{
			m_DecayDecrement = (1.0f - m_SustainValue) / m_SampleRate / decayDuration;
		}

		if (releaseDuration <= 0) 
		{
			m_ReleaseDecrement = 1.0f;
		}
		else 
		{
			m_ReleaseDecrement = 1.0f / m_SampleRate / releaseDuration;
		}
	}

	void LinearEGCore::Render(unsigned int numSamples)
	{
		Update();

		unsigned int totalSamples = numSamples * Audio::Stereo;
		auto outputBuffer = m_Output->GetBuffer();

		for (unsigned int i = 0; i < totalSamples; i+= Audio::Stereo)
		{
			for (unsigned int j = 0; j < Audio::Stereo; j++)
			{
				if (j == 0) 
				{
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
				outputBuffer[i + j] = m_Value;
			}
		}
	}

	void LinearEGCore::DoNoteOn(NoteEvent event) 
	{
		m_ScalarNote = 1.0f - ((float)event.midiNote / 127.0f);
		m_ScalarVelocity = 1.0f - ((float)event.velocity / 127.0f);
		float attackDuration = m_Params->attackDuration * m_ScalarVelocity;
		float decayDuration = m_Params->decayDuration * m_ScalarNote;
		float releaseDuration = m_Params->releaseDuration;
		m_SustainValue = m_Params->sustainValue;

		if (attackDuration <= 0.0f) {
			m_AttackIncrement = 1.0f;
			m_Value = 1.0f;
		} 
		else {
			m_AttackIncrement = 1.0f / m_SampleRate / attackDuration;
			m_Value = -1.0f * m_AttackIncrement;
		}
		
		if (decayDuration <= 0.0f) {
			m_DecayDecrement = 1.0f - m_SustainValue;
		}
		else {
			m_DecayDecrement = (1.0f - m_SustainValue) / m_SampleRate / decayDuration;
		}
		
		if (releaseDuration <= 0.0f) {
			m_ReleaseDecrement = m_SustainValue;
		}
		else {
			m_ReleaseDecrement = m_SustainValue / m_SampleRate / releaseDuration;
		}

		m_State = EGState::Attack;
	}
	
	void LinearEGCore::DoNoteOff(NoteEvent event)
	{
		m_State = EGState::Release;
	}
}