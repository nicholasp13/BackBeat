#pragma once

// Functions and design taken from Will Pirkle's ladder design
// link: https://willpirkle.com/Downloads/Addendum_A1_Moog.pdf

#include "BackBeat/Audio/Instruments/Synth/SynthEngine/ModuleCores/TPTLPLadderFilterCore.h"
#include "BackBeat/Audio/Instruments/Module.h"
namespace BackBeat {

	class LowPassLadderFilter : public Module
	{
	public:
		LowPassLadderFilter(unsigned int sampleRate, unsigned int bufferSize, 
			std::shared_ptr<float[]> buffer, std::shared_ptr<LadderFilterParameters> params);
		~LowPassLadderFilter();

		virtual void Reset(unsigned int sampleRate) override;
		virtual void Update() override;
		virtual void Render(unsigned int numSamples) override;
		virtual void DoNoteOn(NoteEvent event) override;
		virtual void DoNoteOff(NoteEvent event) override;

		inline virtual std::shared_ptr<float[]> GetBuffer() { return m_Buffer; }

		inline std::shared_ptr<float[]> GetModInputBuffer() { return m_ModInput->GetBuffer(); }
		inline ModuleType GetType() { return m_Type; }

	private:
		unsigned int m_BufferSize;
		float m_K; // This is resonance in this filter design 

		std::shared_ptr<LadderFilterParameters> m_Params;
		std::shared_ptr<float[]> m_Buffer;
		std::shared_ptr<TPTLPLadderFilterCore> m_LowPassFilter1;
		std::shared_ptr<TPTLPLadderFilterCore> m_LowPassFilter2;
		std::shared_ptr<TPTLPLadderFilterCore> m_LowPassFilter3;
		std::shared_ptr<TPTLPLadderFilterCore> m_LowPassFilter4;
		std::unique_ptr<Modulator> m_ModInput;

		ModuleType m_Type;

	private:
		void CalculateCoefficients();
		float ProcessSample(float input);

	};
}