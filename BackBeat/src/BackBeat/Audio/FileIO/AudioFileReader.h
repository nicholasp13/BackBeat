#pragma once

#include "BackBeat/Audio/Audio.h"
namespace BackBeat {

	namespace AudioFileReader
	{
		AudioInfo ReadFile(std::string fileName);
		bool ReadAudioFileData(std::string filePath, char* data, unsigned int position, unsigned int numBytes);
		
		static AudioInfo ReadMP3Header(std::string filePath, unsigned int size);
		static AudioInfo ReadHeader(std::string filePath, unsigned int size, FileType type);
	}

}