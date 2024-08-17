#pragma once

/* 
* Basic virtual polyphonic Synth with basic sound waves following MMA DLS level 1 specifications
* Produces only 48k sample rate, 32 bit depth, and stereo sounds for Windows 10 and 11 but 
* other formats are translated by the SynthProcessor (not fully implemented)
*/

// ------------------------------------------------------------------------------- //
// TODO:
// - Create SynthModule Modulation Matrix that has access to EnvelopeGenerators(EGs), 
//       Low Frequency Oscillators(LFOs), and filters to allow for personal
//       customizable sounds
// - Add string names to ModuleCores and way to expose them for GUI purposes
// - Allow for switching SynthVoices' Modules and/or ModuleCores
// - Create other Synth modes like mono, legatto etc. Only polyphonic so far.
// - Create functions to scale units properly with common units like decibels 
//	     for volume
// - Create nonlinear EGs
// - Create other types of oscillators or wave types
// - Add resonance to filters
// - Upgrade to MMA DLS level 2
// - Create method for voice stealing ("stealing" a SynthVoice object already playing 
//       a note to to play a new note on that voice when all voices are active)
// - Create a Linux Renderer to allow for playing on Linux
// - Create an implementation of Fast Fourier Transform and create a way to graph
//   frequency s-plane
// - Reimplement memory by changing most pointers/objects into stack allocation 
//       for improved performance. NOTE: Not necessary but good practice
// 
// NOTE: ModularMatrix on backburner until at least MIDI input devices,
//       multiple audio formats are supported, and string names are added to
//       ModuleCores
// ------------------------------------------------------------------------------- //
#include "SynthBase.h"
#include "BackBeat/Audio/Audio.h"
#include "BackBeat/Audio/MIDI/MIDIInputHandler.h"
#include "BackBeat/Audio/Instruments/Synth/AudioEngine/AudioEngine.h"
#include "BackBeat/Audio/Instruments/Synth/SynthIO/SynthProcessor.h"
#include "BackBeat/Audio/Instruments/Synth/SynthIO/SynthEventHandler.h"
namespace BackBeat {

	class Synth
	{
	public:
		Synth();
		~Synth();

		void Start();
		void Stop();

		inline bool IsRunning() { return m_Running; }
		inline AudioProps GetProps() { return m_Props; }
		inline UUID GetID() { return m_ID; }
		inline std::shared_ptr<SynthEventHandler> GetEventHandler() { return m_Handler; }
		inline std::shared_ptr<MIDIInputHandler> GetMIDIInput() { return m_MIDIInput; }
		inline std::shared_ptr<SynthParameters> GetParams() { return m_Params; }
		inline std::shared_ptr<SynthProcessor> GetProcessor() { return m_Processor; }
	
	private:
		bool m_Running;
		unsigned long m_SleepTime;
		AudioProps m_Props;
		UUID m_ID;

		std::shared_ptr<AudioEngine> m_Engine;
		std::shared_ptr<SynthProcessor> m_Processor;
		std::shared_ptr<SynthEventHandler> m_Handler;
		std::shared_ptr<MIDIInputHandler> m_MIDIInput;
		std::shared_ptr<SynthParameters> m_Params;

	private:
		void InitSynth();
		void Run();
	};
}