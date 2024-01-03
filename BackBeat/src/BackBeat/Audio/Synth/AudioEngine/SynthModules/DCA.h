#pragma once

#include "SynthModule.h"
namespace BackBeat {

	class DCA : public SynthModule
	{
	public:
		DCA(unsigned int bufferSize, std::shared_ptr<float[]> buffer, std::shared_ptr<DCAParameters> params);
		~DCA();

		virtual void Reset(unsigned int sampleRate);
		virtual void Update();
		virtual void Render(unsigned int numSamples);
		virtual void DoNoteOn(NoteEvent event);
		virtual void DoNoteOff(NoteEvent event);

		inline std::shared_ptr<float[]> GetBuffer() { return m_Buffer; }
		inline std::shared_ptr<float[]> GetInputBuffer() { return m_ModInput->GetBuffer(); }
		inline ModuleType GetType() { return m_Type; }

	private:
		unsigned int m_Position;
		float m_LeftAmp;
		float m_RightAmp;
		
		std::shared_ptr<DCAParameters> m_Params;
		std::shared_ptr<float[]> m_Buffer;
		std::unique_ptr<Modulator> m_ModInput;
		ModuleType m_Type;
	};
}