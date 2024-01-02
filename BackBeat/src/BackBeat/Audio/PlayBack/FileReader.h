#pragma once

#include "BackBeat/Audio/Audio.h"
namespace BackBeat {

	class FileReader
	{
	public:
		static AudioInfo CreateFile(std::string fileName);
		
	private:
		static AudioInfo ReadMP3Header(std::string filePath, unsigned int size);
		static AudioInfo ReadWAVHeader(std::string filePath, unsigned int size);
	};
}