#pragma once

#include "AudioData.h"

namespace BackBeat {

	class Loader
	{
	public:
		Loader(tWAVEFORMATEX* deviceProps, AudioData* data);
		~Loader();

		void Start();
		void Stop();
		HRESULT GetData(UINT32 framesAvailable, BYTE* buffer, UINT32* position, 
							bool* playing);
		
		inline UINT32 GetBytePosition() { return m_BytePosition; }
		inline UINT32 GetSamplePosition() { return m_SamplePosition;  }

		bool Loading = false;
		bool Loaded = false;

	private:
		UINT32 m_DataSize;
		UINT32 m_BytePosition;
		UINT32 m_SamplePosition;
		BYTE* m_Data;
		tWAVEFORMATEX m_DeviceProps;
		AudioData* m_DataSrc;

		void Load();
		void LoadWAV(BYTE* bufferTarget, BYTE* bufferSrc, UINT32 framesAvailable);
	};
}