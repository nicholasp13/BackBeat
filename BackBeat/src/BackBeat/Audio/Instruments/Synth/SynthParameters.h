#pragma once

#include "bbpch.h"

// NOTE(s): - Most parameters' ranges are defined in the SynthBase.h file and are based on the
//       MIDI Manufacturer's Association (MMA) Downloadable Sounds (DLS) Specification level 1 and 2
//       documents
//          - Should try and make all of these std::atomic (First attempt did not go well as while
//       it compiled the way I handled GUI by just turning pointers into variables on the stack
//       which is WRITTEN BY the atomic, then WRITTEN BY the ImGui::Widget(), and then WRITES TO
//       the atomic. This caused audio to stop working when the ImGui window for the respective BBObject's
//       was rendered. The audio renderer was still going but audio wasn't being produced. Next try,
//       either make the variables part of the class and keep them as pointers in the widgets, OR
//       retest the old way but going into the debugger to see what happens to the atomic values)

#include "BackBeat/Audio/Wave/Wave.h"
namespace BackBeat {

	// NOTE: Modular structs here temporarily as they are needed for the ModMatrix Parameters but
	//       these will probably be put in another or new header file
	enum ModSources 
	{
		SrcLFOsc1 = 0,
		SrcLFOsc2 = 1,
		SrcEG1 = 2,
		SrcEG2 = 3,
		NumModSources = 4
	};

	enum ModDestinations
	{
		DestOsc1 = 0,
		DestOsc2 = 1,
		DestDCA = 2,
		NumModDestinations = 3
	};

	struct ModularSource 
	{
		float intensity;
	};

	struct ModularDestination 
	{
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
		float volume;
	};

	struct EGParameters
	{
		bool tracking; // Note tracking means that EGS will scale decay time with the note 
		               // i.e. the higher the note the faster the decay. This is common among synths
		               // but some like the MiniMoog do not have tracking
		float attackDuration;
		float decayDuration;
		float releaseDuration;
		float sustainValue;
	};

	struct OscParameters
	{
		float amp;
		float octave;
		float dutyCycle;  // Only used for square wave
		float detune;
		WaveType wave;
	};

	struct LFOParameters
	{
		float amp;
		float hertz;
		float delay;
		WaveType wave;
	};

	struct FilterParameters
	{
		bool isOn;		// This will be controlled by Filter EG when implemented
		float cutoff;	// Wave cutoff in hz
	};

	struct LadderFilterParameters
	{
		bool isOn;
		bool enableAutoLimiter; // Not currently in use
		float cutoff;           // in hz
		float Q;                // GUI resonance variable, range: 1.0 -> 10.0
		float bassBoostPercent; // At 0 this acts as a regular filter
	};

	// Not currently used, may be needed for sound fx or to prevent ladder filters from self oscillating
	struct DetectorParameters
	{
		bool detectDB;
		bool clampToUnityMax;
		int detectMode;
		float attackTime;  // in ms
		float releaseTime; // in ms
	};

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
		std::shared_ptr<LadderFilterParameters> LPLadderFilterParams;
		std::shared_ptr<FilterParameters> HPFilterParams;
		std::shared_ptr<ModMatrixParameters> ModMatrixParams;
	};

	struct EngineParameters
	{
		float volume = 1.0f;
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