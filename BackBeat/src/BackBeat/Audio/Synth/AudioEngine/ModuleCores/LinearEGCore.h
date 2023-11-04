#pragma once

#include "EnvelopeGenerator.h"
namespace BackBeat {

	class LinearEGCore : public EnvelopeGenerator
	{
	public:
		LinearEGCore(UINT32 sampleRate, std::shared_ptr<float[]> buffer, std::shared_ptr<EGParameters> params);
		~LinearEGCore();

		virtual void Reset(UINT32 sampleRate);
		virtual void Update();
		virtual void Render(UINT32 numSamples);
		virtual void DoNoteOn(noteEvent event);
		virtual void DoNoteOff(noteEvent event);

		virtual std::shared_ptr<float[]> GetBuffer() { return m_OutputBuffer; }

		EGState GetState() { return m_State; }
	
	private:
		UINT32 m_SampleRate;
		UINT32 m_InputPosition;
		float m_Value;
		float m_ScalarNote;
		float m_ScalarVelocity;
		float m_AttackIncrement;
		float m_DecayDecrement;
		float m_ReleaseDecrement;
		float m_SustainValue;
		EGState m_State;

		std::shared_ptr<float[]> m_OutputBuffer;
		std::shared_ptr<EGParameters> m_Params;
	};
}