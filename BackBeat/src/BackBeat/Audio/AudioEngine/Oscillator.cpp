#include "bbpch.h"

#include "Oscillator.h"
#include "BackBeat/Audio/Audio.h"

// TODO:
// 1. Create base type Oscillator and different subclasses
// 2. Change is an AudioData to has an AudioData
namespace BackBeat {

	Oscillator::Oscillator(tWAVEFORMATEX props)
		: m_Hertz(A_NOTE), m_Amp(0.5f), m_Props(props)
	{
		m_BufferSize = (UINT32)(m_Props.nSamplesPerSec * m_Props.nChannels); // Change to 1 channel always.
		m_WaveSize = (UINT32)(m_BufferSize / m_Hertz);
		m_Buffer = new float[m_BufferSize];
		InitSinWave();
	}

	Oscillator::~Oscillator()
	{
	}

	// Assumes float buffer type.
	HRESULT Oscillator::LoadBuffer(UINT32 framesAvailable, BYTE* buffer, UINT32* position, bool* loading)
	{
		float* targetBuffer = reinterpret_cast<float*>(buffer);
		
		for (UINT32 i = 0; i < framesAvailable; i++) {
			m_Position = (m_Position + 1) % (m_WaveSize);
			targetBuffer[i] = m_Buffer[m_Position];
		}

		return S_OK;
	}

	void Oscillator::SetProps(tWAVEFORMATEX props) { 
		m_Props = props; 
		InitSinWave();
	}
	
	void Oscillator::SetHertz(float hertz) 
	{ 
		if (hertz < 0.0f)
			m_Hertz = 0.0f;
		else if (hertz > (float)m_Props.nSamplesPerSec)
			m_Hertz = (float)m_Props.nSamplesPerSec;
		else
			m_Hertz = hertz;

		InitSinWave();
		BB_CORE_INFO("CHANGED FREQUENCY {0}", m_Hertz);
	}

	void Oscillator::SetAmp(float amp) 
	{ 
		if (amp < 0.0f)
			m_Amp = 0.0f;
		else if (amp > 1.0f)
			m_Amp = 1.0f;
		else
			m_Amp = amp;

		InitSinWave();
		BB_CORE_INFO("CHANGED AMPLITUDE {0}", m_Amp);
	}

	// Change to 1 channel
	void Oscillator::InitSinWave() 
	{
		m_Position = 0;
		m_WaveSize = (UINT32)(m_BufferSize / m_Hertz);
		SinWave::GetSinWave(m_Buffer, m_WaveSize, m_Props.nChannels, m_Amp);
	}
}