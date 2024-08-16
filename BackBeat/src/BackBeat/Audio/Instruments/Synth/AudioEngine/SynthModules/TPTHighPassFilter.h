#pragma once

#include "BackBeat/Audio/Instruments/Synth/AudioEngine/ModuleCores/TPTHPFilterCore.h"
#include "SynthModule.h"
namespace BackBeat {

	class TPTHighPassFilter : public SynthModule
	{
	public:
		TPTHighPassFilter(unsigned int sampleRate, unsigned int bufferSize, std::shared_ptr<float[]> buffer, std::shared_ptr<FilterParameters> params);
		~TPTHighPassFilter();

		virtual void Reset(unsigned int sampleRate) override;
		virtual void Update() override;
		virtual void Render(unsigned int numSamples) override;
		virtual void DoNoteOn(NoteEvent event) override;
		virtual void DoNoteOff(NoteEvent event) override;

		inline virtual std::shared_ptr<float[]> GetBuffer() override { return m_Core->GetInputBuffer(); }

	private:
		std::shared_ptr<TPTHPFilterCore> m_Core;
	};
}