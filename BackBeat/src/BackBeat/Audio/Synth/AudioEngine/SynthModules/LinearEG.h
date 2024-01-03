#pragma once

#include "EnvelopeGenerator.h"
namespace BackBeat {

	class LinearEG : public EnvelopeGenerator
	{
	public:
		LinearEG(unsigned int sampleRate, unsigned int bufferSize, std::shared_ptr<EGParameters> params);
		~LinearEG();

		virtual void Reset(unsigned int sampleRate);
		virtual void Update();
		virtual void Render(unsigned int numSamples);
		virtual void DoNoteOn(NoteEvent event);
		virtual void DoNoteOff(NoteEvent event);

		virtual std::shared_ptr<float[]> GetBuffer() { return m_Core->GetOutputBuffer(); }

		EGState GetState() { return m_Core->GetState(); }

	private:
		std::shared_ptr<LinearEGCore> m_Core;
	};
}