#pragma once

#include "BackBeat/Audio/Playback/AudioData.h"
#include "WAVData.h"
namespace BackBeat {

	class FileReader
	{
	public:
		static AudioData* CreateFile(std::string fileName);
		
	private:
		static AudioData* ReadMP3Header(std::string filePath, unsigned int size);
		static AudioData* ReadWAVHeader(std::string filePath, unsigned int size);
	};
}