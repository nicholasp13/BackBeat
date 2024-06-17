#pragma once

#include "BackBeat/Audio/Helpers/AudioFileReader.h"
#include "BackBeat/Audio/Helpers/AudioFileWriter.h"
#include "BackBeat/Audio/Helpers/int24.h"
#include "BackBeat/Audio/PlayBack/Track.h"
#include "BackBeat/Core/FileDialog.h"
#include "Sample.h"
namespace BackBeat {

	// TODO: Make a namespace
	namespace SampleBuilder
	{
		void SaveSample(Track* track, unsigned int start, unsigned int end);
		Sample* BuildSample(std::string filePath);

	};
} 