#pragma once

namespace BackBeat {

	class FileReader
	{
	public:
		FileReader() {};
		~FileReader() {};

		static HRESULT GetHeader(int* position, std::string fileName, tWAVEFORMATEX* props);
		
	private:
		static void ReadMP3Header(char* header, tWAVEFORMATEX* props);
		static void ReadWAVHeader(char* header, tWAVEFORMATEX* props);

		static unsigned short EndianConverterShort(char num1, char num2);
		static unsigned long EndianConverterLong(char num1, char num2,
												char num3, char num4);
	};
}