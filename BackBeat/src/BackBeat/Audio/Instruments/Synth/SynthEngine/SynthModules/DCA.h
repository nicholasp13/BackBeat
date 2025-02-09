#pragma once

#include "BackBeat/Audio/Instruments/Module.h"
namespace BackBeat {

	class DCA : public Module
	{
	public:
		DCA(unsigned int bufferSize, std::shared_ptr<float[]> buffer, std::shared_ptr<DCAParameters> params);
		~DCA();

		virtual void Reset(unsigned int sampleRate) override;
		virtual void Update() override;
		virtual void Render(unsigned int numSamples) override;
		virtual void DoNoteOn(NoteEvent event) override;
		virtual void DoNoteOff(NoteEvent event) override;

		inline virtual std::shared_ptr<float[]> GetBuffer() override { return m_Buffer; }

		inline std::shared_ptr<float[]> GetInputBuffer() { return m_ModInput->GetBuffer(); }
		inline ModuleType GetType() { return m_Type; }

	private:
		unsigned int m_Position;
		unsigned int m_BufferSize;
		float m_LeftAmp;
		float m_RightAmp;
		float m_Volume;
		
		std::shared_ptr<DCAParameters> m_Params;
		std::shared_ptr<float[]> m_Buffer;
		std::unique_ptr<Modulator> m_ModInput;

		ModuleType m_Type;
	};
}