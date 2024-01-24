#pragma once

#include "BackBeat/Audio/Synth/AudioEngine/ModuleCores/TPTHPFilterCore.h"
#include "SynthModule.h"
namespace BackBeat {

	class TPTHighPassFilter : public SynthModule
	{
	public:
		TPTHighPassFilter(unsigned int sampleRate, unsigned int bufferSize, std::shared_ptr<float[]> buffer, std::shared_ptr<FilterParameters> params);
		~TPTHighPassFilter();

		virtual void Reset(unsigned int sampleRate);
		virtual void Update();
		virtual void Render(unsigned int numSamples);
		virtual void DoNoteOn(NoteEvent event);
		virtual void DoNoteOff(NoteEvent event);

		inline virtual std::shared_ptr<float[]> GetBuffer() { return m_Core->GetInputBuffer(); }

	private:
		std::shared_ptr<TPTHPFilterCore> m_Core;
	};
}