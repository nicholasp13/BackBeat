#pragma once

// TODO: Create function get a vector/array of string names of ModuleCores to expose to GUI

#include "BackBeat/Audio/Synth/SynthBase.h"
#include "BackBeat/Audio/Synth/AudioEngine/ModuleCores/ModuleCore.h"
namespace BackBeat {

	class SynthModule
	{
	public:
		virtual void Reset(UINT32 sampleRate) {};
		virtual void Update() {};
		virtual void Render(UINT32 numSamples) {};
		virtual void DoNoteOn(NoteEvent event) {};
		virtual void DoNoteOff(NoteEvent event) {};
		
		virtual std::shared_ptr<float[]> GetBuffer() { return std::shared_ptr<float[]>(); }
	};
}