#pragma once

#include "BackBeat/Audio/Helpers/AudioFileWriter.h"
#include "Track.h"
namespace BackBeat {

	class WAVFileBuilder
	{
	public:
		static bool BuildWAVFile(Track* track, unsigned int start, unsigned int end);
	};
}