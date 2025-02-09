#pragma once

#include "BackBeat/Audio/Instruments/Synth/SynthBase.h"
#include "ModuleCore.h"
namespace BackBeat {

	class Module
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