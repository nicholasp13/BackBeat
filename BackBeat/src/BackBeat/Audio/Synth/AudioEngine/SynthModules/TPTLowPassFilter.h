#pragma once

#include "BackBeat/Audio/Synth/AudioEngine/ModuleCores/TPTLPFilterCore.h"
#include "SynthModule.h"
namespace BackBeat {

	class TPTLowPassFilter : public SynthModule
	{
	public:
		TPTLowPassFilter(UINT32 sampleRate, UINT32 bufferSize, std::shared_ptr<float[]> buffer, std::shared_ptr<FilterParameters> params);
		~TPTLowPassFilter();

		virtual void Reset(UINT32 sampleRate);
		virtual void Update();
		virtual void Render(UINT32 numSamples);
		virtual void DoNoteOn(NoteEvent event);
		virtual void DoNoteOff(NoteEvent event);

		virtual std::shared_ptr<float[]> GetBuffer() { return m_Core->GetInputBuffer(); }

	private:
		std::shared_ptr<TPTLPFilterCore> m_Core;
	};
}