#include "bbpch.h"

#include "Renderer.h"
#include "BackBeat/Core/Core.h"

/*
 *
/**/
namespace BackBeat {

	// TODO: CHANGE CONSTRUCTOR TO MATCH THE OBJECT LAYOUT/PATTERN
	Renderer::Renderer(Oscillator* oscillator, tWAVEFORMATEX props)
		: m_Oscillator(oscillator), Loader(&props, oscillator), m_Position(0), m_Props(props)
	{
		m_BufferSize = props.nAvgBytesPerSec;
		m_Buffer = new BYTE[m_BufferSize];
		Loading = true;

		Render();
	}

	Renderer::~Renderer()
	{
	}

	HRESULT Renderer::GetData(UINT32 framesAvailable, BYTE* buffer, UINT32* position,
		bool* playing)
	{
		UINT32 totalSamples = framesAvailable * m_Props.nChannels;
		UINT32 bufferPosition = m_Position;
		UINT32 bufferSize = m_BufferSize / m_Props.nBlockAlign * m_Props.nChannels;
		float* srcBuffer = reinterpret_cast<float*>(m_Buffer);
		float* targetBuffer = reinterpret_cast<float*>(buffer);

		for (UINT32 i = 0; i < totalSamples; i++) {
			if (!playing) {
				m_Position = bufferPosition % m_Oscillator->GetWaveSize();
				return S_OK;
			}
			bufferPosition = (bufferPosition + 1) % (bufferSize);
			targetBuffer[i] = srcBuffer[bufferPosition];
		}

		m_Position = bufferPosition % m_Oscillator->GetWaveSize();
		return S_OK;
	}

	void Renderer::Render()
	{
		m_Position = 0;
		m_Oscillator->LoadBuffer(m_Props.nSamplesPerSec * m_Props.nChannels, m_Buffer, &m_Position, &Loading);
		Loading = false;
		Loaded = true;
	}
}