#pragma once

// TODO: Create function get a vector/array of string names of ModuleCores to expose to GUI

#include "BackBeat/Audio/Instruments/Synth/SynthBase.h"
#include "BackBeat/Audio/Instruments/Synth/AudioEngine/ModuleCores/ModuleCore.h"
namespace BackBeat {

	class SynthModule
	{
	public:
		virtual void Reset(unsigned int sampleRate) {};
		virtual void Update() {};
		virtual void Render(unsigned int numSamples) {};
		virtual void DoNoteOn(NoteEvent event) {};
		virtual void DoNoteOff(NoteEvent event) {};
		
		virtual std::shared_ptr<float[]> GetBuffer() { return std::shared_ptr<float[]>(); }
	};
}