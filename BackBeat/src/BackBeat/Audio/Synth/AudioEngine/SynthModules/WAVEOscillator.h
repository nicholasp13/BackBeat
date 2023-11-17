#pragma once

//  TODO: Change way this class stores cores. Might change to a single core

#include "SynthModule.h"
#include "BackBeat/Audio/Synth/AudioEngine/ModuleCores/WaveOscCore.h"
namespace BackBeat {

	class WaveOscillator : public SynthModule
	{
	public:
		WaveOscillator(UINT32 sampleRate, std::shared_ptr<float[]> buffer, std::shared_ptr<OscParameters> params);
		~WaveOscillator();

		virtual void Reset(UINT32 sampleRate);
		virtual void Update();
		virtual void Render(UINT32 numSamples);
		virtual void DoNoteOn(noteEvent event);
		virtual void DoNoteOff(noteEvent event);

		virtual std::shared_ptr<float[]> GetBuffer() { return m_Core->GetBuffer(); }
		std::shared_ptr<float[]> GetModInputBuffer() { return m_Core->GetInputBuffer(); }

	private:
		std::shared_ptr<WaveOscCore> m_Core;

	};
}