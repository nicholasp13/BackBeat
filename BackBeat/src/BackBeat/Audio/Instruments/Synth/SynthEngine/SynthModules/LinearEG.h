#pragma once

#include "EnvelopeGenerator.h"
namespace BackBeat {

	class LinearEG : public EnvelopeGenerator
	{
	public:
		LinearEG(unsigned int sampleRate, unsigned int bufferSize, std::shared_ptr<float[]> outputBuffer,
			std::shared_ptr<EGParameters> params);
		~LinearEG();

		virtual void Reset(unsigned int sampleRate) override;
		virtual void Update() override;
		virtual void Render(unsigned int numSamples) override;
		virtual void DoNoteOn(NoteEvent event) override;
		virtual void DoNoteOff(NoteEvent event) override;

		inline virtual std::shared_ptr<float[]> GetBuffer() override { return m_Core->GetOutputBuffer(); }
		inline virtual EGState GetState() override { return m_Core->GetState(); }

	private:
		std::shared_ptr<LinearEGCore> m_Core;
		std::shared_ptr<float[]> m_InputBuffer;
		std::shared_ptr<float[]> m_OutputBuffer;
	};
}