#pragma once

#include "BackBeat/Audio/Audio.h"
namespace BackBeat {

	class AudioFileWriter
	{
	public:
		static bool WriteWAVFileHeader(std::string filePath, AudioProps props, unsigned long dataSize);
		static bool WriteWAVFileData(std::string filePath, char* data, unsigned int numBytes, unsigned int position);

	};
}