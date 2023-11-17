#pragma once

#include "EnvelopeGenerator.h"
namespace BackBeat {

	class LinearEG : public EnvelopeGenerator
	{
	public:
		LinearEG(UINT32 sampleRate, UINT32 bufferSize, std::shared_ptr<EGParameters> params);
		~LinearEG();

		virtual void Reset(UINT32 sampleRate);
		virtual void Update();
		virtual void Render(UINT32 numSamples);
		virtual void DoNoteOn(noteEvent event);
		virtual void DoNoteOff(noteEvent event);

		virtual std::shared_ptr<float[]> GetBuffer() { return m_Core->GetOutputBuffer(); }

		EGState GetState() { return m_Core->GetState(); }

	private:
		std::shared_ptr<LinearEGCore> m_Core;
	};
}