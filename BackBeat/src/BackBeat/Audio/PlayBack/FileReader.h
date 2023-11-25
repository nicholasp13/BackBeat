#pragma once

#include "BackBeat/Audio/Playback/AudioData.h"
#include "WAVData.h"
namespace BackBeat {

	class FileReader
	{
	public:
		static HRESULT CreateFile(std::string fileName, AudioData** audioData);
		
	private:
		static void ReadMP3Header(char* header, tWAVEFORMATEX* props, unsigned long* size);
		static void ReadWAVHeader(char* header, tWAVEFORMATEX* props, unsigned long* size);
	};
}