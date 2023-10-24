#pragma once

#include "ModuleCore.h"
#include "Wave.h"
#include "BackBeat/Audio/Synth/SynthBase.h"
namespace BackBeat {
	
	class Oscillator : public ModuleCore
	{
	public:
		Oscillator(UINT32 sampleRate, UINT32 bufferSize, std::shared_ptr<float[]> buffer, WaveType wave);
		~Oscillator();

		virtual void Reset(UINT32 sampleRate);
		virtual void Update();
		virtual void Render(UINT32 numSamples);
		virtual void DoNoteOn(noteEvent event);
		virtual void DoNoteOff(noteEvent event);

		std::shared_ptr<float[]> GetBuffer() { return m_Buffer; }

	private:
		UINT32 m_SampleRate;
		UINT32 m_BufferSize;
		UINT32 m_Position;
		UINT32 m_WaveSize;
		float m_Amp;
		std::shared_ptr<float[]> m_Buffer;
		float* m_Wave;
		WaveType m_WaveType;
		ModuleType m_Type;

		void InitWave(float hertz, float amp);
	};
}