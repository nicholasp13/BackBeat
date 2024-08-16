#pragma once

// TODO: Add string names to module cores to expose the cores to GUI

#include "bbpch.h"
#include "BackBeat/Audio/Instruments/Synth/AudioEngine/ModularMatrix/Modulator.h"
#include "BackBeat/Audio/Instruments/Synth/SynthBase.h"
namespace BackBeat {

	// NOTE: This enum class is not used currently and maybe unneccsary. 
	enum class ModuleType
	{
		None = 0,
		WaveOscCore, 
		Filter,
		LowPassFilter,
		HighPassFilter,
		LFO,
		EG,
		DCA
	};

	class ModuleCore
	{
	public:
		virtual void Reset(unsigned int sampleRate) {}
		virtual void Update() {}
		virtual void Render(unsigned int numSamples) {}
		virtual void DoNoteOn(NoteEvent event) {}
		virtual void DoNoteOff(NoteEvent event) {}
		virtual std::shared_ptr<float[]> GetBuffer() { return std::shared_ptr<float[]>(); }
	};
}