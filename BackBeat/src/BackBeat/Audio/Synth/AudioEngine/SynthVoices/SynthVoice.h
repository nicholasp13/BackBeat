#pragma once

#include "BackBeat/Audio/Synth/Render/RenderInfo.h"
#include "BackBeat/Audio/Synth/AudioEngine/SynthModules/SynthModule.h"
#include "BackBeat/Audio/Synth/AudioEngine/SynthModules/DCA.h"
#include "BackBeat/Audio/Synth/AudioEngine/SynthModules/WaveOscillator.h"
#include "BackBeat/Audio/Synth/AudioEngine/SynthModules/LinearEG.h"
#include "BackBeat/Audio/Synth/AudioEngine/SynthModules/AmpEG.h"
#include "BackBeat/Audio/Synth/AudioEngine/SynthModules/LowFrequencyOscillator.h"
#include "BackBeat/Audio/Synth/SynthBase.h"
namespace BackBeat {

	class SynthVoice
	{
	public:
		SynthVoice(UINT32 sampleRate, std::shared_ptr<float[]> outputBuffer, std::shared_ptr<VoiceParameters> params);
		~SynthVoice();

		// Virtual for implementation of subclasses if needed
		virtual void Reset(UINT32 sampleRate);
		virtual void Update();
		virtual void Render(UINT32 numSamples);
		virtual void ProcessMIDIEvent(midiEvent event);

		virtual bool IsActive() { return m_Actice; }

		int GetChannel() { return m_Channel; }

	private:
		UINT32 m_SampleRate;
		bool m_Actice;
		int m_Channel;

		std::shared_ptr<VoiceParameters> m_Params;
		std::shared_ptr<float[]> m_InputBuffer;
		std::shared_ptr<float[]> m_OutputBuffer;
		std::unique_ptr<DCA> m_DCA;
		std::unique_ptr<WaveOscillator> m_Osc1;
		std::unique_ptr<WaveOscillator> m_Osc2;
		std::unique_ptr<WaveOscillator> m_Osc3;
		std::unique_ptr<WaveOscillator> m_Osc4;
		std::unique_ptr<LinearEG> m_EG;
		std::unique_ptr<AmpEG> m_AmpEG;
		std::unique_ptr<LowFrequencyOscillator> m_LFO1;

		// TODO: Implement filter and modulation matrix
		// std::unique_ptr<SynthModule> m_Filter;

		void DoNoteOn(noteEvent event);
		void DoNoteOff(noteEvent event);
		void InitModules();
	};
}