#pragma once

#include "BackBeat/Audio/Synth/SynthBase.h"
#include "BackBeat/Audio/Synth/AudioEngine/ModuleCores/ModuleCore.h"
namespace BackBeat {

	class SynthModule
	{
	public:
		virtual void Reset(UINT32 sampleRate) {};
		virtual void Update() {};
		virtual void Render(UINT32 numSamples) {};
		virtual void DoNoteOn(noteEvent event) {};
		virtual void DoNoteOff(noteEvent event) {};
		
		virtual std::shared_ptr<float[]> GetBuffer() { return std::shared_ptr<float[]>(); }
	};
}