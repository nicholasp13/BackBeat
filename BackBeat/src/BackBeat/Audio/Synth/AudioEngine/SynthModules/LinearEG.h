#pragma once

#include "BackBeat/Audio/Synth/AudioEngine/ModuleCores/LinearEGCore.h"
#include "SynthModule.h"
namespace BackBeat {

	class LinearEG : public SynthModule
	{
	public:
		LinearEG(UINT32 sampleRate, UINT32 bufferSize, std::shared_ptr<float[]> buffer);
		~LinearEG();

		virtual void Reset(UINT32 sampleRate);
		virtual void Update();
		virtual void Render(UINT32 numSamples);
		virtual void DoNoteOn(noteEvent event);
		virtual void DoNoteOff(noteEvent event);

		virtual std::shared_ptr<float[]> GetBuffer() { return m_Buffer; }

		EGState GetState() { return m_Core->GetState(); }

	private:
		UINT32 m_BufferSize;
		UINT32 m_SampleRate;
		std::shared_ptr<float[]> m_Buffer;
		std::shared_ptr<LinearEGCore> m_Core;
	};
}