#pragma once

#include "ModuleCore.h"
#include "BackBeat/Audio/Helpers/Wave.h"
#include "BackBeat/Audio/Instruments/Synth/SynthBase.h"
namespace BackBeat {

	class WaveOscCore : public ModuleCore
	{
	public:
		WaveOscCore(unsigned int sampleRate, std::shared_ptr<float[]> buffer, std::shared_ptr<OscParameters> params);
		~WaveOscCore();

		virtual void Reset(unsigned int sampleRate) override;
		virtual void Update() override;
		virtual void Render(unsigned int numSamples) override;
		virtual void DoNoteOn(NoteEvent event) override;
		virtual void DoNoteOff(NoteEvent event) override;

		inline std::shared_ptr<float[]> GetBuffer() { return m_Buffer; }
		inline std::shared_ptr<float[]> GetInputBuffer() { return m_ModInput->GetBuffer(); }
		 
	private:
		static const unsigned int s_BufferSize = 100000;
		
		unsigned int m_SampleRate;
		unsigned int m_Position;
		unsigned int m_WaveSize;
		float m_Amp;
		float m_Hertz;
		WaveType m_WaveType;
		ModuleType m_Type;
		std::shared_ptr<float[]> m_Wave;

		std::shared_ptr<float[]> m_Buffer;
		std::shared_ptr<OscParameters> m_Params;
		std::unique_ptr<Modulator> m_ModInput;

		void InitWave();
	};
}