#pragma once

#include "ModuleCore.h"
#include "BackBeat/Audio/Synth/AudioEngine/Wave.h"
#include "BackBeat/Audio/Synth/SynthBase.h"
namespace BackBeat {
	
	class WaveOscCore : public ModuleCore
	{
	public:
		WaveOscCore(UINT32 sampleRate, std::shared_ptr<float[]> buffer, std::shared_ptr<OscParameters> params);
		~WaveOscCore();

		virtual void Reset(UINT32 sampleRate);
		virtual void Update();
		virtual void Render(UINT32 numSamples);
		virtual void DoNoteOn(NoteEvent event);
		virtual void DoNoteOff(NoteEvent event);

		std::shared_ptr<float[]> GetBuffer() { return m_Buffer; }
		std::shared_ptr<float[]> GetInputBuffer() { return m_ModInput->GetBuffer(); }
		 
	private:
		UINT32 m_SampleRate;
		UINT32 m_Position;
		UINT32 m_WaveSize;
		float m_Amp;
		float m_Hertz;
		WaveType m_WaveType;
		ModuleType m_Type;

		std::shared_ptr<float[]> m_Buffer;
		std::shared_ptr<OscParameters> m_Params;
		std::unique_ptr<Modulator> m_ModInput;
		float* m_Wave;

		void InitWave();
	};
}