#pragma once

namespace BackBeat {

	class FileReader
	{
	public:
		FileReader() {};
		~FileReader() {};

		static bool ReadFile(std::string fileName, tWAVEFORMATEX* props);
		
	private:
		static void ReadMP3Header(char* header, tWAVEFORMATEX* props);
		static void ReadWAVHeader(char* header, tWAVEFORMATEX* props);
	};
}