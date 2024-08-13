#pragma once

/**
* Basic Resistor-Capacitor Low Pass Filter represented by a 1-pole topology-preserving transform (TPT) based on the 
* trapezoidal integrator replacement method using a bilinear transform. Follows the design by Vadim Zavalishin in The Art of VA Filter Design
* section 3.10.
* NOTE: There is no prewarping to more closely match analog filters as the difference is negligible audibly. May be implemented later 
*       Prewarping cutoff equation: m_CutoffFreq = tan(m_Params->cutoff / m_SampleRate / 2);
*/

// TODO: Add resonance

#include "ModuleCore.h"
namespace BackBeat {

	class  TPTLPFilterCore : public ModuleCore
	{
	public:
		TPTLPFilterCore(unsigned int sampleRate, unsigned int bufferSize, std::shared_ptr<float[]> buffer, std::shared_ptr<FilterParameters> params);
		~TPTLPFilterCore();

		virtual void Reset(unsigned int sampleRate) override;
		virtual void Update() override;
		virtual void Render(unsigned int numSamples) override;
		virtual void DoNoteOn(NoteEvent event) override;
		virtual void DoNoteOff(NoteEvent event) override;

		inline std::shared_ptr<float[]> GetInputBuffer() { return m_Input->GetBuffer(); }

	private:
		bool m_IsOn; // NOTE: Current implementation does not follow DLS 2 specifications where it states that the m_CutOffFreq be set to 0x7FFFFFFF for no low pass filter
		unsigned int m_SampleRate;
		unsigned int m_OutputBufferSize;
		float m_Gain; // Represented by T in the transform method, defaulted to 1 and not subject to change yet
		float m_CutoffFreq;
		float m_Resonance; // Not implemented
		
		// Helper Variables used in transform method
		float m_G; // G = g / ( 1 + g), g = cutoff freq * constant T / 2 where T is usually 1 / sample rate and some gain
		float m_S; // Contains the state of z^-1 which is the last output of the transform 
		
		ModuleType m_Type;

		std::shared_ptr<FilterParameters> m_Params;
		std::unique_ptr<Modulator> m_Input; // Not implemented
		std::shared_ptr<float[]> m_OutputBuffer;
	};
}