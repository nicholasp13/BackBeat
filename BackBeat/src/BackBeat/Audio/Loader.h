#pragma once

#include "AudioData.h"

namespace BackBeat {

	class Loader
	{
	public:
		Loader(tWAVEFORMATEX* deviceProps, AudioData* data);
		~Loader();

		void Start();
		HRESULT Stop();
		HRESULT GetData(UINT32 framesAvailable, BYTE* buffer, UINT32* position, 
							bool* playing);
		
	private:
		boolean m_Loading;
		UINT32 m_DataSize;
		BYTE* m_Data;
		tWAVEFORMATEX m_DeviceProps;
		AudioData* m_DataSrc;

		void Load();
		HRESULT LoadWAV(BYTE* bufferTarget, BYTE* bufferSrc, UINT32 framesAvailable);
	};
}