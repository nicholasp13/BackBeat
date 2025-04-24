#pragma once

#include "BackBeat/Audio/FileIO/AudioFileReader.h"
#include "BackBeat/Audio/FileIO/AudioFileWriter.h"
#include "BackBeat/Audio/PlayBack/Track.h"
#include "BackBeat/Core/int24.h"
#include "BackBeat/File/FileDialog.h"
#include "Sample.h"
namespace BackBeat {

	namespace SampleBuilder
	{
		void SaveSample(std::shared_ptr<Track> track, unsigned int start, unsigned int end);
		void SaveSample(AudioProps props, float* leftBuffer, float* rightBuffer, unsigned int numSamples);
		std::shared_ptr<Sample> BuildSample(std::string filePath);

	};
}