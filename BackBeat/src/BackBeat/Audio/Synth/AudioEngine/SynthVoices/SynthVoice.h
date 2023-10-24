#pragma once

#include "BackBeat/Audio/Synth/Render/RenderInfo.h"
#include "BackBeat/Audio/Synth/AudioEngine/SynthModules/SynthModule.h"
#include "BackBeat/Audio/Synth/AudioEngine/SynthModules/DCA.h"
#include "BackBeat/Audio/Synth/AudioEngine/SynthModules/WAVEOscillator.h"
#include "BackBeat/Audio/Synth/AudioEngine/SynthModules/LinearEG.h"
#include "BackBeat/Audio/Synth/SynthBase.h"
namespace BackBeat {

	class SynthVoice
	{
	public:
		SynthVoice(std::shared_ptr<float[]> outputBuffer, UINT32 sampleRate, UINT32 bufferSize);
		~SynthVoice();

		// Virtual for implementation of subclasses if needed
		virtual void Reset(UINT32 sampleRate);
		virtual void Update();
		virtual void Render(UINT32 numSamples);
		virtual void ProcessMIDIEvent(midiEvent event);

		virtual bool IsActive() { return m_Actice; }

		int GetChannel() { return m_Channel; }

	private:
		bool m_Actice;
		int m_Channel;
		UINT32 m_SampleRate;

		std::shared_ptr<float[]> m_InputBuffer;
		std::shared_ptr<float[]> m_OutputBuffer;
		std::unique_ptr<DCA> m_DCA;
		std::unique_ptr<WAVEOscillator> m_Oscillator;
		std::unique_ptr<LinearEG> m_EG;

		// TODO: Implement filter and modulation matrix
		// std::unique_ptr<SynthModule> m_Filter;
		// ModularMatrix (NOTE: Update SynthModule to create

		void DoNoteOn(noteEvent event);
		void DoNoteOff(noteEvent event);
	};
}