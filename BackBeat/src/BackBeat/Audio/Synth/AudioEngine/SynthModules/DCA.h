#pragma once

#include "SynthModule.h"
namespace BackBeat {

	class DCA : public SynthModule
	{
	public:
		DCA(UINT32 bufferSize, std::shared_ptr<float[]> buffer, std::shared_ptr<DCAParameters> params);
		~DCA();

		virtual void Reset(UINT32 sampleRate);
		virtual void Update();
		virtual void Render(UINT32 numSamples);
		virtual void DoNoteOn(noteEvent event);
		virtual void DoNoteOff(noteEvent event);

		std::shared_ptr<float[]> GetBuffer() { return m_Buffer; }
		std::shared_ptr<float[]> GetInputBuffer() { return m_ModInput->GetBuffer(); }
		ModuleType GetType() { return m_Type; }

	private:
		UINT32 m_Position;
		float m_LeftAmp;
		float m_RightAmp;
		
		std::shared_ptr<DCAParameters> m_Params;
		std::shared_ptr<float[]> m_Buffer;
		std::unique_ptr<Modulator> m_ModInput;
		ModuleType m_Type;
	};
}