#pragma once

#include "BackBeat/Audio/Loader.h"
#include "Oscillator.h"

namespace BackBeat {

	class Renderer : public Loader
	{
	public:
		Renderer(Oscillator* oscillator, tWAVEFORMATEX props);
		~Renderer();

		void Start() {}
		void Stop() {}

		HRESULT GetData(UINT32 framesAvailable, BYTE* buffer, UINT32* position,
			bool* playing);
		void Render();

	private:
		UINT32 m_Position;
		UINT32 m_BufferSize;
		Oscillator* m_Oscillator;
		BYTE* m_Buffer;
		tWAVEFORMATEX m_Props;
	};
}