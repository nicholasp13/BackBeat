#pragma once

#include "BackBeat/Audio/Audio.h"
namespace BackBeat {

	namespace AudioFileReader
	{
		AudioInfo ReadFile(std::string fileName);
		
		static AudioInfo ReadMP3Header(std::string filePath, unsigned int size);
		static AudioInfo ReadHeader(std::string filePath, unsigned int size, FileType type);
	}

}