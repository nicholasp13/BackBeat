#pragma once

#include "BackBeat/Audio/Helpers/AudioFileReader.h"
#include "BackBeat/Audio/Helpers/AudioFileWriter.h"
#include "BackBeat/Audio/Helpers/int24.h"
#include "BackBeat/Audio/PlayBack/Track.h"
#include "BackBeat/Core/FileDialog.h"
#include "Sample.h"
namespace BackBeat {

	namespace SampleBuilder
	{
		void SaveSample(std::shared_ptr<Track> track, unsigned int start, unsigned int end);
		Sample* BuildSample(std::string filePath);

	};
} 