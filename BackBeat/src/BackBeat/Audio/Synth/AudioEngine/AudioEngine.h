#pragma once

/*
* Basic virtual Synth Engine that implements architecture based on Will C. Pirkle's SynthLab SDK with his
* textbook "Designing Software Synthesizer PlugIns in C++":
* - The Engine is composed of an array of SynthVoices.
* - SynthVoices are composed of SynthModules like Digitally Controlled Amplifier (DCA) and Oscillators (Osc)
* - SynthModules is a wrapper class for at most 4 ModuleCore that do the real work
* - ModuleCores provide the actual work i.e. Osc cores calculating the samples for a basic sine wave.
* This allows for programming highly customizable Synths. Only basic ModuleCores have been implemented so far.
* --- SDK Website: https://www.willpirkle.com/synthlab/docs/html/index.html ---
 */

#include <queue>

#include "BackBeat/Audio/Synth/SynthBase.h"
#include "BackBeat/Audio/Synth/Render/RenderInfo.h"
#include "BackBeat/Audio/Synth/AudioEngine/SynthVoices/SynthVoice.h"
namespace BackBeat {

#define MAX_VOICES 16

	class AudioEngine
	{
	public:
		AudioEngine(tWAVEFORMATEX props);
		~AudioEngine();

		virtual void Reset(UINT32 sampleRate);
		virtual void Render(std::shared_ptr<RenderInfo> info);
		virtual void SetParam(); // TODO: Implement
		virtual void GetParam(); // TODO: Implement
		virtual void ProcessMIDIEvent(midiEvent event);

	private:
		UINT32 m_NumVoices;
		UINT32 m_OutputBufferPosition;
		float m_VoiceFactor;
		tWAVEFORMATEX m_Props;

		std::shared_ptr<float[]> m_Buffer;
		std::unique_ptr<SynthVoice> m_Voices[MAX_VOICES];
		std::queue<midiEvent> m_MIDIEvents;

		void InitVoices();
		void FlushBuffer(UINT32 numSamples);
	};
}