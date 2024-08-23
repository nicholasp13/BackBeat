#pragma once

// -- BUG: --
// Very noticeable artifacting in Synth. This happens only in Synth as other BackBeat audio creators
// do not cause any artifacting even as the Synth is. The artifacting is most noticeable in held notes/chords. While
// it is not completely unbearable it is still not up to par with how a standard DAW should run.
// HOWEVER, at this moment I will be moving on from trying to solve this.
// Possible solutions:
//  - Multithreading each SynthVoice
//  - Seperating the callback of WindowsRenderer and the AudioEngine (tried multiple times)
//  - Double buffering and pre-loading the AudioEngine Render such that WindowsRenderer only needs to copy the buffer
//    (requires multithreading)
//  
// NOTES: Looking at CPU usage profiler, the biggest use is the m_Voices->Render() calls in the SynthEngine
//        Further in the callstack shows that it is mostly the Osc cores that cause most (~50%) of the CPU
//        usage in SynthVoice.Render()

/* 
* Basic virtual polyphonic Synth with basic sound waves following MMA DLS level 1 specifications
* Produces only 48k sample rate, 32 bit depth, and stereo sounds for Windows 10 and 11 but 
* other formats are translated by the SynthProcessor (not fully implemented)
*/

// ------------------------------------------------------------------------------- //
// TODO:
// - Fix/reduce artifacting
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
#include "BackBeat/Audio/Instruments/Synth/SynthEngine/SynthEngine.h"
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

		std::shared_ptr<SynthEngine> m_Engine;
		std::shared_ptr<SynthProcessor> m_Processor;
		std::shared_ptr<SynthEventHandler> m_Handler;
		std::shared_ptr<MIDIInputHandler> m_MIDIInput;
		std::shared_ptr<SynthParameters> m_Params;

	private:
		void InitSynth();
		void Run();
	};
}