#pragma once

// TODO:
// Rename to OscillatorCore for consisntency

#include "ModuleCore.h"
#include "Wave.h"
#include "BackBeat/Audio/Synth/SynthBase.h"
namespace BackBeat {
	
	class Oscillator : public ModuleCore
	{
	public:
		Oscillator(UINT32 sampleRate, std::shared_ptr<float[]> buffer, std::shared_ptr<OscParameters> params);
		~Oscillator();

		virtual void Reset(UINT32 sampleRate);
		virtual void Update();
		virtual void Render(UINT32 numSamples);
		virtual void DoNoteOn(noteEvent event);
		virtual void DoNoteOff(noteEvent event);

		std::shared_ptr<float[]> GetBuffer() { return m_Buffer; }

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
		float* m_Wave;

		void InitWave();
	};
}