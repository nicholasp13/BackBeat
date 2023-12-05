#pragma once

// TODO: Create an EGCore interface

#include "ModuleCore.h"
namespace BackBeat {

	enum class EGState
	{
		Off = 0,
		Attack,
		Decay,
		Sustain,
		Release
	};

	class LinearEGCore : public ModuleCore
	{
	public:
		LinearEGCore(UINT32 sampleRate, UINT32 bufferSize, std::shared_ptr<EGParameters> params);
		~LinearEGCore();

		virtual void Reset(UINT32 sampleRate);
		virtual void Update();
		virtual void Render(UINT32 numSamples);
		virtual void DoNoteOn(NoteEvent event);
		virtual void DoNoteOff(NoteEvent event);

		virtual std::shared_ptr<float[]> GetInputBuffer() { return m_Input->GetBuffer(); }
		virtual std::shared_ptr<float[]> GetOutputBuffer() { return m_Output->GetBuffer(); }

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

		std::unique_ptr<Modulator> m_Input;
		std::unique_ptr<Modulator> m_Output;
		std::shared_ptr<EGParameters> m_Params;
	};
}