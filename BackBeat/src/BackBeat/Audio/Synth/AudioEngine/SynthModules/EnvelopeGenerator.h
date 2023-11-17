#pragma once

#include "BackBeat/Audio/Synth/AudioEngine/ModuleCores/LinearEGCore.h"
#include "SynthModule.h"
namespace BackBeat {

	class EnvelopeGenerator : public SynthModule
	{
	public:
		virtual void Reset(UINT32 sampleRate) {}
		virtual void Update() {}
		virtual void Render(UINT32 numSamples) {}
		virtual void DoNoteOn(noteEvent event) {}
		virtual void DoNoteOff(noteEvent event) {}
		virtual std::shared_ptr<float[]> GetBuffer() { return std::shared_ptr<float[]>(); }

		virtual EGState GetState() { return EGState::Off; }
	};
}