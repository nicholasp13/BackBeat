#include "bbpch.h"

#include "BackBeat/Audio/Helpers/AudioFileReader.h"
#include "TrackFactory.h"
namespace BackBeat {

	Track* TrackFactory::BuildTrack(std::string filePath)
	{
		AudioInfo info = AudioFileReader::ReadFile(filePath);
		if (info.type != none) {
			Track* track = new Track(info);
			return track;
		}

		return nullptr;
	}
}