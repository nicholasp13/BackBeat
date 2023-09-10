#pragma once

#include "BackBeat/Audio/AudioData.h"
#include "SinWave.h"

// TODO:
// 1. Create base type Oscillator and different subclasses
// 2. Change is an AudioData to has an AudioData
namespace BackBeat {
	
	class Oscillator : public AudioData
	{
	public:
		Oscillator(tWAVEFORMATEX props);
		~Oscillator();

		virtual HRESULT LoadBuffer(UINT32 framesAvailable, BYTE* buffer, UINT32* position, bool* loading);
		virtual tWAVEFORMATEX* GetProperties() { return &m_Props; }

		void SetProps(tWAVEFORMATEX props);
		void SetHertz(float hertz);
		void SetAmp(float amp);

		UINT32 GetBufferSize() { return m_BufferSize; }
		UINT32 GetWaveSize() { return m_WaveSize; }
		float GetHertz() { return m_Hertz; }
		float GetAmp() { return m_Amp; }

	private:
		UINT32 m_BufferSize;
		UINT32 m_WaveSize;
		UINT32 m_Position;
		float m_Hertz;
		float m_Amp;
		float* m_Buffer;
		tWAVEFORMATEX m_Props;

		void InitSinWave();
	};
}