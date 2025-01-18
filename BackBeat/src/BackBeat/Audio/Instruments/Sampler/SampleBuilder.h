#pragma once

#include "BackBeat/Audio/FileIO/AudioFileReader.h"
#include "BackBeat/Audio/FileIO/AudioFileWriter.h"
#include "BackBeat/Audio/Helpers/int24.h"
#include "BackBeat/Audio/PlayBack/Track.h"
#include "BackBeat/File/FileDialog.h"
#include "Sample.h"
namespace BackBeat {

	namespace SampleBuilder
	{
		void SaveSample(std::shared_ptr<Track> track, unsigned int start, unsigned int end);
		std::shared_ptr<Sample> BuildSample(std::string filePath);

	};
} 