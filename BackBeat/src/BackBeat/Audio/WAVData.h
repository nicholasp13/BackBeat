#pragma once

#include "AudioData.h"

namespace BackBeat {

	class WAVData : public AudioData
	{
	public:
		WAVData(std::string filePath, tWAVEFORMATEX* props);
		~WAVData();

	private:
		HRESULT CreateFile();

		BYTE* m_Data;
		unsigned int m_FileSize;
		std::string m_FilePath;
		tWAVEFORMATEX m_Props;

		struct ConvertedProps
		{
			unsigned short	FormatTag, NumChannels, BlockAlign,
							BitsPerSample, FileSize;
			unsigned long	SamplesPerSec, BytesPerSec;
		};
	};
}