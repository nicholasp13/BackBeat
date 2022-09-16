#include "bbpch.h"

#include "FileReader.h"
#include "BackBeat/Core/Core.h"
namespace BackBeat {

	std::string FileReader::ReadFile(std::string filePath) 
	{
		
		std::ifstream file;
		std::string value = "FAILED";

		const char* mp3 = "ID3";

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
				value = header;
				BB_CORE_INFO("MP3 File opened");
			}
			else if (std::strcmp(WAV, header) == 0)
			{
				value = header;
				BB_CORE_INFO("WAV File opened");
			}
		}
		return value;
	}
}