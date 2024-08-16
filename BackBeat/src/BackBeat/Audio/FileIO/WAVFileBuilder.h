#pragma once

#include "BackBeat/Audio/FileIO/AudioFileWriter.h"
#include "BackBeat/Audio/PlayBack/Track.h"
namespace BackBeat {

	namespace WAVFileBuilder
	{
		bool BuildWAVFile(Track* track, unsigned int start, unsigned int end);
	}
}