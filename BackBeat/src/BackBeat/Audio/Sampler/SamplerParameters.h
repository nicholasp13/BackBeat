#pragma once

#include "bbpch.h"

#include "BackBeat/Audio/Synth/SynthParameters.h"
namespace BackBeat {

	struct SamplerEngineParameters
	{
		float volume;
		std::vector< std::shared_ptr<DCAParameters> > DCAParams;
	};

	struct SamplerParameters
	{
		std::shared_ptr<SamplerEngineParameters> engineParams;
	};
}