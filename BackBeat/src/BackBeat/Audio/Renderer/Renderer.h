#pragma once

#include "BackBeat/Audio/IO/Mixer.h"
#include "BackBeat/Audio/IO/AudioSink.h"
namespace BackBeat {

	class Renderer
	{
	public:
		virtual void Start() {}
		virtual void Stop() {}
		virtual bool IsRendering() { return true; }
		virtual Mixer* GetMixer() { return nullptr; }
	};
}