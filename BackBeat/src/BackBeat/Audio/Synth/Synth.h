#pragma once

/* 
* Basic virtual polyphonic Synth with basic sound waves.
* Only produces 48k sample rate and stereo sounds for Windows 10 and 11.
* Other formats and Linux may be implemented later
*/

// ------------------------------------------------------------------------------- //
// TODO:
// - Create SynthModule Modulation Matrix that has access to EnvelopeGenerators(EGs), 
//       Low Frequency Oscillators(LFOs), and other types of filters to allow for
//       real time customizable sounds
// - Create SubModules and ModuleCores for filters, LFOs, etc.
// - Create classes and objects that allow for simple to implement GUI contols
//       for Modulation Matrix and possibly switching SynthVoices' moduleCores
// - Allow of MIDI input devices to be read and programmed with sounds in this
//      Engine
// - Create class to write data to a file to save and to play
// - Create other Synth modes like mono, legatto etc. Only polyphonic so far.
// - Create a Linux Renderer to allow for playing on Linux
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

		void InitSynth();
		void Run();
		void SyncBuffers();
	};
}