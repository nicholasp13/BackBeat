#pragma once

#include "AudioData.h"
#include "WAVData.h"

namespace BackBeat {

	class FileReader
	{
	public:
		FileReader() {};
		~FileReader() {};

		static HRESULT CreateFile(std::string fileName, AudioData* audioData);
		
	private:
		static void ReadMP3Header(char* header, tWAVEFORMATEX* props);
		static void ReadWAVHeader(char* header, tWAVEFORMATEX* props);

		static unsigned short EndianConverterShort(char num1, char num2);
		static unsigned long EndianConverterLong(char num1, char num2,
												char num3, char num4);
	};
}