#pragma once

#include "bbpch.h"
#include "BackBeat/Audio/Synth/SynthBase.h"
namespace BackBeat {

	enum class ModuleType
	{
		None = 0,
		Oscillator,
		Filter,
		DCA
	};

	class ModuleCore
	{
	public:
		virtual void Reset(UINT32 sampleRate) {}
		virtual void Update() {}
		virtual void Render(UINT32 numSamples) {}
		virtual void DoNoteOn(noteEvent event) {}
		virtual void DoNoteOff(noteEvent event) {}
		virtual std::shared_ptr<float[]> GetBuffer() { return std::shared_ptr<float[]>(); }
	};
}