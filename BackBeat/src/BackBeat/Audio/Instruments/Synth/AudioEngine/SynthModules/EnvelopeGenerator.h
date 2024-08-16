#pragma once

#include "BackBeat/Audio/Instruments/Synth/AudioEngine/ModuleCores/LinearEGCore.h"
#include "SynthModule.h"
namespace BackBeat {

	class EnvelopeGenerator : public SynthModule
	{
	public:
		virtual void Reset(unsigned int sampleRate) {}
		virtual void Update() {}
		virtual void Render(unsigned int numSamples) {}
		virtual void DoNoteOn(NoteEvent event) {}
		virtual void DoNoteOff(NoteEvent event) {}
		virtual std::shared_ptr<float[]> GetBuffer() { return std::shared_ptr<float[]>(); }
		virtual EGState GetState() { return EGState::Off; }
	};
}