#pragma once

#include "BackBeat/Audio/Instruments/Synth/SynthEngine/ModuleCores/NoiseGeneratorCore.h"
#include "BackBeat/Audio/Instruments/Module.h"
namespace BackBeat {

	class NoiseGenerator : public Module
	{
	public:
		NoiseGenerator(
			unsigned int sampleRate, 
			unsigned int bufferSize, 
			std::shared_ptr<float[]> outputBuffer, 
			std::shared_ptr<NoiseGeneratorParameters> params);
		~NoiseGenerator();

		virtual void Reset(unsigned int sampleRate) override;
		virtual void Update() override;
		virtual void Render(unsigned int numSamples) override;
		virtual void DoNoteOn(NoteEvent event) override;
		virtual void DoNoteOff(NoteEvent event) override;

		inline virtual std::shared_ptr<float[]> GetBuffer() override { return nullptr; }

	private:
		std::shared_ptr<NoiseGeneratorCore> m_Core;

	};
}