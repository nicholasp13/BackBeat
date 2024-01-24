#pragma once

#include "BackBeat/Audio/Helpers/AudioFileReader.h"
#include "BackBeat/Audio/Helpers/AudioFileWriter.h"
#include "BackBeat/Audio/Helpers/int24.h"
#include "BackBeat/Audio/PlayBack/Track.h"
#include "BackBeat/Core/FileDialog.h"
#include "Sample.h"
namespace BackBeat {

	class SampleBuilder
	{
	public:
		static void SaveSample(Track* track, unsigned int start, unsigned int end);
		static Sample* BuildSample(std::string filePath);

	};
} 