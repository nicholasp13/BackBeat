#pragma once

#include "Mixer.h"
namespace BackBeat {

	class Renderer
	{
	public:
		virtual void Start() {};
		virtual void Stop() {};
		virtual bool IsRendering() { return true; };
		virtual std::shared_ptr<Mixer> GetMixer() { return std::shared_ptr<Mixer>(); };
	};
}