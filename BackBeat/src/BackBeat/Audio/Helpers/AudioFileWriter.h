#pragma once

#include "BackBeat/Audio/Audio.h"
namespace BackBeat {

	// TODO: Change to return unsigned int error codes instead of just false
	namespace AudioFileWriter
	{
		bool WriteWAVFileHeader(std::string filePath, AudioProps props, unsigned long dataSize);
		bool WriteSampleFileHeader(std::string filePath, AudioProps props, unsigned int dataSize);
		bool WriteAudioFileData(std::string filePath, char* data, unsigned int numBytes);
	}

}