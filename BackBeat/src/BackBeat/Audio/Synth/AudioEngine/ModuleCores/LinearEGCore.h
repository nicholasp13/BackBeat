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
		LinearEGCore(unsigned int sampleRate, unsigned int bufferSize, std::shared_ptr<EGParameters> params);
		~LinearEGCore();

		virtual void Reset(unsigned int sampleRate);
		virtual void Update();
		virtual void Render(unsigned int numSamples);
		virtual void DoNoteOn(NoteEvent event);
		virtual void DoNoteOff(NoteEvent event);

		inline virtual std::shared_ptr<float[]> GetInputBuffer() { return m_Input->GetBuffer(); }
		inline virtual std::shared_ptr<float[]> GetOutputBuffer() { return m_Output->GetBuffer(); }

		inline EGState GetState() { return m_State; }
	
	private:
		unsigned int m_SampleRate;
		unsigned int m_InputPosition;
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