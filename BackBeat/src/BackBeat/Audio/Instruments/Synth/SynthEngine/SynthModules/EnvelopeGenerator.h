#pragma once

#include "BackBeat/Audio/Instruments/Synth/SynthEngine/ModuleCores/LinearEGCore.h"
#include "BackBeat/Audio/Instruments/Module.h"
namespace BackBeat {

	class EnvelopeGenerator : public Module
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