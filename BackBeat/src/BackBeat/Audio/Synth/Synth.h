#pragma once

/* 
* Basic virtual polyphonic Synth with basic sound waves following MMA DLS level 1 specifications
* Only produces 48k sample rate, 32 bit depth, and stereo sounds for Windows 10 and 11.
* Other formats and Linux may be implemented later
*/

// ------------------------------------------------------------------------------- //
// TODO:
// - Create SynthModule Modulation Matrix that has access to EnvelopeGenerators(EGs), 
//       Low Frequency Oscillators(LFOs), and filters to allow for personal
//       customizable sounds
// - Add string names to ModuleCores and way to expose them for GUI purposes
// - Allow for switching SynthVoices' Modules and/or ModuleCores
// - Allow of MIDI input devices to be read and programmed with sounds in this
//       Engine <- NEXT TO BE IMPLEMENTED
// - Create class to write data to a file to save and to play
// - Create other Synth modes like mono, legatto etc. Only polyphonic so far.
// - Create functions to scale units properly with common units like decibels 
//	     for volume
// - Create nonlinear EGs
// - Create other types of oscillators or wave types
// - Upgrade to MMA DLS level 2
// - Create method for voice stealing ("stealing" a SynthVoice object already playing 
//       a note to to play a new note on that voice when all voices are active)
// - Create a Linux Renderer to allow for playing on Linux
// - Create an implementation of Fast Fourier Transform and create a way to graph
//   frequency s-plane
// 
// NOTE: ModularMatrix on backburner until at least MIDI input devices,
//       multiple audio formats are supported, and string names are added to
//       ModuleCores
// ------------------------------------------------------------------------------- //
#include "SynthBase.h"
#include "BackBeat/Audio/Audio.h"
#include "BackBeat/Audio/Synth/AudioEngine/AudioEngine.h"
#include "BackBeat/Audio/Synth/Render/WindowsRenderer.h"
#include "BackBeat/Audio/Synth/SynthIO/SynthEventHandler.h"
namespace BackBeat {

	class Synth
	{
	public:
		Synth();
		~Synth();

		void Start();
		void Stop();

		boolean IsRunning() { return m_Running; }
		std::shared_ptr<Renderer> GetRenderer() { return m_Renderer; }
		std::shared_ptr<SynthEventHandler> GetEventHandler() { return m_Handler; }
		std::shared_ptr<SynthParameters> GetParams() { return m_Params; }
	
	private:
		boolean m_Running;
		UINT32 m_SamplesToRender;
		DWORD m_SleepTime;
		tWAVEFORMATEX m_Props;
		std::thread m_Thread;
		std::shared_ptr<Timer> m_Timer;
		std::shared_ptr<AudioEngine> m_Engine;
		std::shared_ptr<Renderer> m_Renderer;
		std::shared_ptr<SynthEventHandler> m_Handler;
		std::shared_ptr<RenderInfo> m_RenderInfo;
		std::shared_ptr<SynthParameters> m_Params;

		void InitSynth();
		void Run();
	};
}