#pragma once

#include "ModuleCore.h"
namespace BackBeat {

	enum class EGState
	{
		Off = 0,
		Attack,
		Decay,
		Sustain,
		Release
	};

	class EnvelopeGenerator : public ModuleCore
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