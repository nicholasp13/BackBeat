#pragma once

#include "AudioData.h"

namespace BackBeat {

	class WAVData : public AudioData
	{
	public:
		WAVData(std::string filePath,tWAVEFORMATEX* props,int size);
		~WAVData();

		virtual HRESULT FormatStream(tWAVEFORMATEX* deviceProps);
		virtual HRESULT LoadBuffer(UINT32 framesAvailable, BYTE* buffer, unsigned int* position, DWORD* flag);
		virtual FileType GetFileType();

	private:
		unsigned long m_Size = 0;
		std::ifstream m_Data;
		std::string m_FilePath = "";
		tWAVEFORMATEX m_Props;

		struct ConvertedProps
		{
			unsigned short	FormatTag, NumChannels, BlockAlign,
							BitsPerSample, FileSize;
			unsigned long	SamplesPerSec, BytesPerSec;
		};
	};
}