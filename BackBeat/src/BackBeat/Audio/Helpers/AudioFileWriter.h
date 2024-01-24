#pragma once

#include "BackBeat/Audio/Audio.h"
namespace BackBeat {

	class AudioFileWriter
	{
	public:
		// TODO: Change to unsigned int
		static bool WriteWAVFileHeader(std::string filePath, AudioProps props, unsigned long dataSize);
		static bool WriteSampleFileHeader(std::string filePath, AudioProps props, unsigned int dataSize);
		static bool WriteAudioFileData(std::string filePath, char* data, unsigned int numBytes);

	};
}