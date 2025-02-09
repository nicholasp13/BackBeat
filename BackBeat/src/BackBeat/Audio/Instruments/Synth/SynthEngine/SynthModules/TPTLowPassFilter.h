#pragma once

// NOTE: DEPRECIATED use LowPassLadderFilter instead for the more interesting filter sound

#include "BackBeat/Audio/Instruments/Synth/SynthEngine/ModuleCores/TPTLPFilterCore.h"
#include "BackBeat/Audio/Instruments/Module.h"
namespace BackBeat {

	class TPTLowPassFilter : public Module
	{
	public:
		TPTLowPassFilter(unsigned int sampleRate, unsigned int bufferSize, std::shared_ptr<float[]> buffer, std::shared_ptr<FilterParameters> params);
		~TPTLowPassFilter();

		virtual void Reset(unsigned int sampleRate) override;
		virtual void Update() override;
		virtual void Render(unsigned int numSamples) override;
		virtual void DoNoteOn(NoteEvent event) override;
		virtual void DoNoteOff(NoteEvent event) override;

		inline virtual std::shared_ptr<float[]> GetBuffer() { return m_Core->GetInputBuffer(); }

	private:
		std::shared_ptr<TPTLPFilterCore> m_Core;
	};
}