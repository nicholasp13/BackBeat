#include "bbpch.h"

#include "BackBeat/Audio/FileIO/AudioFileReader.h"
#include "TrackFactory.h"
namespace BackBeat {

	static unsigned long s_DefaultFileMapSize = 1073741824ul; // 1 gb (placeholder, maybe place somewhere else)
	static FileMapper* s_FileMapper = nullptr;

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

	std::shared_ptr<MappedTrack> TrackFactory::BuildMappedTrack(std::string filePath)
	{
		if (!s_FileMapper)
			return nullptr;

		AudioInfo info = AudioFileReader::ReadFile(filePath);
		if (info.type == FileType::wav)
			return std::make_shared<MappedTrack>(info, s_FileMapper->MapFile(filePath, info.props.fileSize));

		return nullptr;
	}

	std::shared_ptr<MappedTrack> TrackFactory::BuildMappedTrack(AudioInfo info)
	{
		if (!s_FileMapper)
			return nullptr;

		if (info.type == FileType::wav)
			return std::make_shared<MappedTrack>(info, s_FileMapper->MapFile(info.filePath, info.props.fileSize));

		return nullptr;
	}

	// TODO: Make Write() a base class function so that this can be any type of track
	// Regular non-mapped tracks will need an empty file as standard c++ file I/O does not allow for overwrites
	void TrackFactory::CopyTrackData(std::shared_ptr<Track> input, std::shared_ptr<Track> output)
	{
		if (!input || !output)
			return;
		if (input->GetProps() != output->GetProps())
			return;

		input->SetPosition(0);
		output->SetPosition(0);

		const unsigned int bufferSize = 4800; // Must match block align checks
		bool done = false;
		byte buffer[bufferSize] = {};
		// Tracks checks the bufferSize and checks if it is done 
		while (!done)
		{
			if (!input->Read(buffer, bufferSize))
				return;
			if (!output->Write(buffer, bufferSize))
				return;
			done = input->IsDone();
		}

	}

	void TrackFactory::SetFileMapper(FileMapper* fileMapper)
	{
		if (fileMapper)
			s_FileMapper = fileMapper;
	}

	void TrackFactory::SetFileSize(unsigned long size)
	{
		s_DefaultFileMapSize = size;
	}

}