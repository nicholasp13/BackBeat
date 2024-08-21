#pragma once

// Envelope Generator dedicated specifically to the DCA for envelope control of note amplitude
// All voices have this EG and it is always routed to the DCA and cannot be changed

// TODO: Allow for swapping to other EG cores when/if made

#include "EnvelopeGenerator.h"
namespace BackBeat {

	class AmpEG : public EnvelopeGenerator
	{
	public:
		AmpEG(unsigned int sampleRate, unsigned int bufferSize, std::shared_ptr<float[]> DCABuffer, std::shared_ptr<EGParameters> params);
		~AmpEG();

		virtual void Reset(unsigned int sampleRate) override;
		virtual void Update() override;
		virtual void Render(unsigned int numSamples) override;
		virtual void DoNoteOn(NoteEvent event) override;
		virtual void DoNoteOff(NoteEvent event) override;

		inline virtual std::shared_ptr<float[]> GetBuffer() override { return m_InputBuffer; }
		inline virtual EGState GetState() override { return m_Core->GetState(); }

	private:
		std::shared_ptr<LinearEGCore> m_Core; 
		std::shared_ptr<float[]> m_InputBuffer;
		std::shared_ptr<float[]> m_OutputBuffer;
	};
}