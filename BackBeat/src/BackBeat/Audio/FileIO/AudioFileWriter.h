#pragma once

#include "BackBeat/Audio/Audio.h"
namespace BackBeat {

	namespace AudioFileWriter
	{
		bool WriteWAVFileHeader(std::string filePath, AudioProps props, unsigned long dataSize);
		bool WriteSampleFileHeader(std::string filePath, AudioProps props, unsigned int dataSize);
		bool WriteAudioFileData(std::string filePath, char* data, unsigned int numBytes);
		bool WriteAudioFileSilence(std::string filePath, unsigned int numBytes);
	}

}