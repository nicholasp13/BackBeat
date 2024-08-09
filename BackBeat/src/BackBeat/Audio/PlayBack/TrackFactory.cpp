#include "bbpch.h"

#include "BackBeat/Audio/Helpers/AudioFileReader.h"
#include "TrackFactory.h"
namespace BackBeat {

	std::shared_ptr<Track> TrackFactory::BuildTrack(std::string filePath)
	{
		AudioInfo info = AudioFileReader::ReadFile(filePath);
		if (info.type == FileType::wav)
			return std::make_shared<Track>(info);

		return nullptr;
	}
}