#pragma once

// TODO: Remove sample rate and buffersize from parameters

#include "bbpch.h"

#include "AudioEngine/ModuleCores/Wave.h"
namespace BackBeat {

	enum ModSources {
		SrcLFOsc1 = 0,
		SrcLFOsc2 = 1,
		SrcEG1 = 2,
		SrcEG2 = 3,
		NumModSources = 4
	};

	enum ModDestinations {
		DestOsc1 = 0,
		DestOsc2 = 1,
		DestDCA = 2,
		NumModDestinations = 3
	};

	struct ModularSource {
		float intensity;
	};

	struct ModularDestination {
		float intensity;
		bool channelOn[NumModSources];
	};

	struct ModMatrixParameters
	{
		float channelIntensity[NumModSources];
		std::shared_ptr< ModularSource[NumModSources] > modSourcesRows;
		std::shared_ptr< ModularDestination[NumModDestinations] > modDestinationsCols;
	};

	struct DCAParameters
	{
		float leftAmp;
		float rightAmp;
	};

	struct EGParameters
	{
		float attackDuration;
		float decayDuration;
		float releaseDuration;
		float sustainValue;
	};

	struct OscParameters
	{
		float amp;
		WaveType wave;
	};

	// TODO: Multiple params for multiple of the same core
	struct VoiceParameters
	{
		std::shared_ptr<DCAParameters> DCAParams;
		std::shared_ptr<EGParameters> EGParams;
		std::shared_ptr<OscParameters> OscParams;
		std::shared_ptr<ModMatrixParameters> ModMatrixParams;
	};

	struct EngineParameters
	{
		float volume;
		std::shared_ptr<VoiceParameters> voiceParams;
	};

	struct EventHandlerParameters
	{
		int octave;
		byte noteVelocity;
	};

	// TODO: Add event handler params when event handler params is implemented
	struct SynthParameters
	{
		std::shared_ptr<EngineParameters> engineParams;
		std::shared_ptr<EventHandlerParameters> eventHandlerParams;
	};
}