#pragma once

/*
* Basic virtual Synth Engine that implements architecture based on Will C. Pirkle's SynthLab SDK with his
* textbook "Designing Software Synthesizer PlugIns in C++":
* - The Engine is composed of an array of SynthVoices.
* - SynthVoices are composed of SynthModules like Digitally Controlled Amplifier (DCA) and Oscillators (Osc)
* - SynthModules is a wrapper class for at most 4 ModuleCore that do the real work
* - ModuleCores provide the actual work i.e. Osc cores calculating the samples for a basic sine wave.
* The engine takes MIDI input from the main Synth class and calls its SynthVoices to produce the required sound
* by descending function calls to DoNoteOn() and Render(). The engine sums the voices output into a single buffer
* shared with the Renderer that works with the Windows audio API (Linux support on the table) This architecture
* allows for programming highly customizable Synths. Only basic ModuleCores have been implemented so far.
* --- SDK Website: https://www.willpirkle.com/synthlab/docs/html/index.html ---
* 
* NOTE: All calculations and output are in STEREO, 32 bit depth (float), 48k sample rate properties. Data output is loaded
*       into a single float buffer with the left channel indexed on even indeces and the right channel on odd indeces. 
*       Synth Renderer will be responsible for Audio format conversions if necessary (not currently implemented)
 */

#include <queue>

#include "BackBeat/Audio/Instruments/Synth/SynthBase.h"
#include "BackBeat/Audio/Renderer/RenderInfo.h"
#include "BackBeat/Audio/Instruments/Synth/SynthEngine/SynthVoices/SynthVoice.h"
namespace BackBeat {

	constexpr unsigned int SynthMaxVoices = 12;

	class SynthEngine
	{
	public:
		SynthEngine(AudioProps props);
		~SynthEngine();
		
		void Stop();

		virtual void Reset(unsigned int sampleRate);
		virtual void Render(std::shared_ptr<RenderInfo> info);
		virtual void SetParam(); // TODO: Implement if needed
		virtual std::shared_ptr<EngineParameters> GetParam();
		virtual void ProcessMIDIEvent(MIDIEvent event);

	private:
		unsigned int m_NumVoices;
		float m_VoiceFactor;
		AudioProps m_Props;

		std::shared_ptr<float[]> m_Buffer;
		std::shared_ptr<EngineParameters> m_Params;
		std::unique_ptr<SynthVoice> m_Voices[SynthMaxVoices];
		std::queue<MIDIEvent> m_MIDIEvents;

	private:
		void InitVoices();
		void InitParameters();
	};
}