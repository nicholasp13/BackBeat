#pragma once

#include "AudioFileWriter.h"
#include "Track.h"
namespace BackBeat {

	class AudioFileBuilder
	{
	public:
		static bool BuildWAVFile(Track* track, unsigned int start, unsigned int end);
	};
}