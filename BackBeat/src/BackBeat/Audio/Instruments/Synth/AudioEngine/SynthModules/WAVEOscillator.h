#pragma once

#include "SynthModule.h"
#include "BackBeat/Audio/Instruments/Synth/AudioEngine/ModuleCores/WaveOscCore.h"
namespace BackBeat {

	class WaveOscillator : public SynthModule
	{
	public:
		WaveOscillator(unsigned int sampleRate, std::shared_ptr<float[]> buffer, std::shared_ptr<OscParameters> params);
		~WaveOscillator();

		virtual void Reset(unsigned int sampleRate) override;
		virtual void Update() override;
		virtual void Render(unsigned int numSamples) override;
		virtual void DoNoteOn(NoteEvent event) override;
		virtual void DoNoteOff(NoteEvent event) override;

		inline virtual std::shared_ptr<float[]> GetBuffer() override { return m_Core->GetBuffer(); }
		inline std::shared_ptr<float[]> GetModInputBuffer() { return m_Core->GetInputBuffer(); }

	private:
		std::shared_ptr<WaveOscCore> m_Core;

	};
}