#pragma once

#include "bbpch.h"

#include "AudioEngine/Wave.h"
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

	// TODO: 
	//	Add pitch shifting by cents
	//	Add panning to individual oscillators	
	struct OscParameters
	{
		float amp;
		float octave;
		WaveType wave;
	};

	// TODO:
	//	Add delay in LFO start
	struct LFOParameters
	{
		float amp;
		float hertz;
		WaveType wave;
	};

	// TODO: Add params for LFOs, filters, and other SynthModules as coded
	struct VoiceParameters
	{
		std::shared_ptr<DCAParameters> DCAParams;
		std::shared_ptr<EGParameters> AmpEGParams;
		std::shared_ptr<EGParameters> EGParams;
		std::shared_ptr<LFOParameters> LFOParams1;
		std::shared_ptr<OscParameters> OscParams1;
		std::shared_ptr<OscParameters> OscParams2;
		std::shared_ptr<OscParameters> OscParams3;
		std::shared_ptr<OscParameters> OscParams4;
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

	struct SynthParameters
	{
		std::shared_ptr<EngineParameters> engineParams;
		std::shared_ptr<EventHandlerParameters> eventHandlerParams;
	};
}