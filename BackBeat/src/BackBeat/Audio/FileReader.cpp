#include "bbpch.h"

#include "FileReader.h"
#include "BackBeat/Core/Core.h"
namespace BackBeat {

#define MP3 "ID3"
#define WAV "RIF"

	bool FileReader::ReadFile(std::string filePath, tWAVEFORMATEX* props) 
	{
		
		std::ifstream file;

		// TODO: Implement Function to get mp3/wav file header info
		file.open(filePath, std::ios::binary);
		if (file.is_open())
		{
			char* header = new char[4];
			file.read(header, 3);
			file.close();
			header[3] = '\0';
			
			if (std::strcmp(MP3, header) == 0)
			{
				BB_CORE_INFO("MP3 File opened");
				return true;
			}
			else if (std::strcmp(WAV, header) == 0)
			{
				BB_CORE_INFO("WAV File opened");
				return true;
			}
		}
		BB_CORE_ERROR("FILE FAILED TO LOAD");
		return false;
	}

	void ReadMP3Header(std::string filePath, tWAVEFORMATEX* props) {

	}

	void ReadWAVHeader(std::string filePath, tWAVEFORMATEX* props) {

	}
}