#pragma once

#include "EnvelopeGenerator.h"
namespace BackBeat {

	class LinearEGCore : public EnvelopeGenerator
	{
	public:
		LinearEGCore(UINT32 sampleRate, UINT32 bufferSize, std::shared_ptr<float[]> outputBuffer);
		~LinearEGCore();

		virtual void Reset(UINT32 sampleRate);
		virtual void Update();
		virtual void Render(UINT32 numSamples);
		virtual void DoNoteOn(noteEvent event);
		virtual void DoNoteOff(noteEvent event);

		virtual std::shared_ptr<float[]> GetBuffer() { return m_OutputBuffer; }

		void SetAttack(float duration) { m_AttackDuration = duration; }
		void SetDecay(float duration) { m_DecayDuration = duration; }
		void SetRelease(float duration) { m_ReleaseDuration = duration; }

		EGState GetState() { return m_State; }
	
	private:
		UINT32 m_InputPosition;
		UINT32 m_OutputSize;
		UINT32 m_SampleRate;
		float m_Value;
		float m_Scalar;
		float m_SustainValue;
		float m_AttackIncrement;
		float m_DecayDecrement;
		float m_ReleaseDecrement;
		float m_AttackDuration;
		float m_DecayDuration;
		float m_ReleaseDuration;
		EGState m_State;

		std::shared_ptr<float[]> m_OutputBuffer;
	};
}