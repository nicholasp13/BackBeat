#include "bbpch.h"

#include "BackBeat/Audio/FileIO/AudioFileReader.h"
#include "TrackFactory.h"
namespace BackBeat {

	std::shared_ptr<Track> TrackFactory::BuildTrack(std::string filePath)
	{
		AudioInfo info = AudioFileReader::ReadFile(filePath);
		if (info.type == FileType::wav)
			return std::make_shared<Track>(info);

		return nullptr;
	}

	std::shared_ptr<Track> TrackFactory::BuildTempTrack(UUID id, AudioProps props)
	{
		auto filePath = FileSystem::GetTempDir().c_str() + id.ToString();
		AudioInfo info = {
			.type = FileType::recordingTemp,
			.name = id.ToString(),
			.filePath = filePath,
			.props = props,
			.dataZero = 0u,
			.dataSize = 0u
		};

		return std::make_shared<Track>(info);
	}

}