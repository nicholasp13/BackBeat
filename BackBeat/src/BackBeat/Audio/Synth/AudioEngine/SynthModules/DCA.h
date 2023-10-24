#pragma once

#include "SynthModule.h"
namespace BackBeat {

	class DCA : public SynthModule
	{
	public:
		DCA(UINT32 sampleRate, UINT32 bufferSize, std::shared_ptr<float[]> buffer);
		~DCA();

		virtual void Reset(UINT32 sampleRate);
		virtual void Update();
		virtual void Render(UINT32 numSamples);
		virtual void DoNoteOn(noteEvent event);
		virtual void DoNoteOff(noteEvent event);

		std::shared_ptr<float[]> GetBuffer() { return m_Buffer; }
		void SetRightAmp(float amp) { m_RightAmp = amp; }
		void SetLefttAmp(float amp) { m_LeftAmp = amp; }
		ModuleType GetType() { return m_Type; }

	private:
		UINT32 m_SampleRate;
		UINT32 m_BufferSize;
		UINT32 m_Position;
		float m_RightAmp;
		float m_LeftAmp;
		std::shared_ptr<float[]> m_Buffer;
		ModuleType m_Type;
	};
}