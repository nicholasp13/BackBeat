#pragma once

#include "BackBeat/Audio/IO/Mixer.h"
#include "BackBeat/Audio/IO/AudioSink.h"
namespace BackBeat {

	class Renderer
	{
	public:
		virtual void Start() {}
		virtual void Stop() {}

		// NOTE: Two seperate Render function calls as found with WindowsApi WASAPI, the render timing/loop
		// should be done inside the WindowsRenderer class as it causes errors and major audio artifacting.
		// Regular Render() is not currently used but is still here in case future Renderer's APIs can be run
		// with an outside loop/thread
		virtual void Render() {}
		// NOTE: See above comments
		virtual void RenderFree() {}

		virtual bool IsRendering() { return true; }
		virtual Mixer* GetMixer() { return nullptr; }
		// NOTE: Should be in nanoseconds
		virtual long long GetCycleTime() { return 0; }
		virtual void SetMixer(Mixer* mixer) {}
	};
}