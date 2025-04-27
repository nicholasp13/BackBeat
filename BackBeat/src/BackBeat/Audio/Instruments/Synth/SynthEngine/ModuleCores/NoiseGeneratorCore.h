#pragma once

#include "BackBeat/Audio/Instruments/ModuleCore.h"
namespace BackBeat {

	// NOTE: Might not need to fill entire modulator array with values since LFO values vary little due to low freq
	class NoiseGeneratorCore : public ModuleCore
	{
	public:
		NoiseGeneratorCore(
			unsigned int sampleRate, 
			unsigned int bufferSize, 
			std::shared_ptr<float[]> outputBuffer,
			std::shared_ptr<NoiseGeneratorParameters> params);
		~NoiseGeneratorCore();

		virtual void Reset(unsigned int sampleRate) override;
		virtual void Update() override;
		virtual void Render(unsigned int numSamples) override;
		virtual void DoNoteOn(NoteEvent event) override;
		virtual void DoNoteOff(NoteEvent event) override;

	private:
		bool m_On;
		unsigned int m_SampleRate;
		float m_Level;

		std::shared_ptr<float[]> m_InputBuffer;
		std::shared_ptr<float[]> m_OutputBuffer;
		std::shared_ptr<NoiseGeneratorParameters> m_Params;

	};

}