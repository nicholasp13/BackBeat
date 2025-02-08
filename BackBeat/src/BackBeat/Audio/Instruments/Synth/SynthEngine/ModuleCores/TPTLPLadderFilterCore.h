#pragma once

// Functions and design taken from Will Pirkle's ladder design
// link: https://willpirkle.com/Downloads/Addendum_A1_Moog.pdf

#include "BackBeat/Audio/Instruments/ModuleCore.h"
namespace BackBeat {

	class  TPTLPLadderFilterCore : public ModuleCore
	{
	public:
		TPTLPLadderFilterCore(unsigned int sampleRate);
		~TPTLPLadderFilterCore();

		virtual void Reset(unsigned int sampleRate) override;
		virtual void Update() override;
		virtual void Render(unsigned int numSamples) override;
		virtual void DoNoteOn(NoteEvent event) override;
		virtual void DoNoteOff(NoteEvent event) override;

		float ProcessSample(float input);

		inline float GetG_Value() { return m_G; } // GetGValue() is taken by a macro in a random header file
		inline float GetS_Value() { return m_S; }
		inline void SetCuttoffFreq(float cuttoff) { m_CutoffFreq = cuttoff; }

	private:
		unsigned int m_SampleRate;
		float m_Gain; // Represented by T in the transform method, defaulted to 1 and not subject to change yet
		float m_CutoffFreq;

		// Helper Variables used in transform method
		float m_G; // G = g / ( 1 + g), g = tan( 2 * pi * cutoff freq * constant T / 2) where T is usually 1 / sample rate and some gain
		float m_S; // Contains the state of z^-1 which is the last output of the transform 

		ModuleType m_Type;

	};
}