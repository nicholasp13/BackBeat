#pragma once

#include "BackBeat/Audio/Synth/AudioEngine/ModuleCores/LFOCore.h"
#include "SynthModule.h"
namespace BackBeat {

	class LowFrequencyOscillator : public SynthModule
	{
	public:
		LowFrequencyOscillator(unsigned int sampleRate, unsigned int bufferSize, std::shared_ptr<float[]> outputBuffer, std::shared_ptr<LFOParameters> params);
		~LowFrequencyOscillator();

		virtual void Reset(unsigned int sampleRate);
		virtual void Update();
		virtual void Render(unsigned int numSamples);
		virtual void DoNoteOn(NoteEvent event);
		virtual void DoNoteOff(NoteEvent event);

		virtual std::shared_ptr<float[]> GetBuffer() { return std::shared_ptr<float[]>(); }

	private:
		std::shared_ptr<LFOCore> m_Core;
		std::shared_ptr<float[]> m_InputBuffer;
		std::shared_ptr<float[]> m_OutputBuffer;
	};
}