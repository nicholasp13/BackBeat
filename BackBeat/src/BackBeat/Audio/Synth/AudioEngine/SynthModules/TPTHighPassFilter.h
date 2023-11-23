#pragma once

#include "BackBeat/Audio/Synth/AudioEngine/ModuleCores/TPTHPFilterCore.h"
#include "SynthModule.h"
namespace BackBeat {

	class TPTHighPassFilter : public SynthModule
	{
	public:
		TPTHighPassFilter(UINT32 sampleRate, UINT32 bufferSize, std::shared_ptr<float[]> buffer, std::shared_ptr<FilterParameters> params);
		~TPTHighPassFilter();

		virtual void Reset(UINT32 sampleRate);
		virtual void Update();
		virtual void Render(UINT32 numSamples);
		virtual void DoNoteOn(noteEvent event);
		virtual void DoNoteOff(noteEvent event);

		virtual std::shared_ptr<float[]> GetBuffer() { return m_Core->GetInputBuffer(); }

	private:
		std::shared_ptr<TPTHPFilterCore> m_Core;
	};
}