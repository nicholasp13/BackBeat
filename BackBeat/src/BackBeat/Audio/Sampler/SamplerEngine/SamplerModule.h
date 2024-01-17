#pragma once

#include "SamplerCore.h"
#include "BackBeat/Audio/Synth/AudioEngine/SynthModules/SynthModule.h"
namespace BackBeat {

	class SamplerModule : public SynthModule
	{
	public:
		SamplerModule(unsigned int sampleRate, std::shared_ptr<float[]> buffer);
		~SamplerModule();

		virtual void Reset(unsigned int sampleRate);
		virtual void Update();
		virtual void Render(unsigned int numSamples);
		virtual void DoNoteOn(NoteEvent event);
		virtual void DoNoteOff(NoteEvent event);

		inline void SetSample(Sample* newSample) { m_Core->SetSample(newSample); }
		inline std::string GetName() { return m_Core->GetName(); }
		inline std::shared_ptr<SamplerCore> GetCore() { return m_Core; }

	private:
		std::shared_ptr<SamplerCore> m_Core;

	};
}