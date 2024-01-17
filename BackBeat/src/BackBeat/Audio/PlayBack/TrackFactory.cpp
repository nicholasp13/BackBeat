#include "bbpch.h"

#include "BackBeat/Audio/Helpers/AudioFileReader.h"
#include "TrackFactory.h"
namespace BackBeat {

	Track* TrackFactory::BuildTrack(std::string filePath)
	{
		AudioInfo info = AudioFileReader::ReadFile(filePath);
		if (info.type == wav)
			return new Track(info);

		return nullptr;
	}
}