#pragma once

#include "BackBeat/Core/UUID.h"
#include "BackBeat/File/FileSystem.h"
#include "BackBeat/File/FileMapper.h"
#include "MappedTrack.h"
#include "Track.h"
namespace BackBeat {

	namespace TrackFactory
	{
		std::shared_ptr<Track> BuildTrack(std::string filePath);
		std::shared_ptr<Track> BuildTempTrack(UUID id, AudioProps props);
		std::shared_ptr<MappedTrack> BuildMappedTrack(std::string filePath);
		std::shared_ptr<MappedTrack> BuildMappedTrack(AudioInfo info);
		void CopyTrackData(std::shared_ptr<Track> input, std::shared_ptr<Track> output);
		void SetFileMapper(FileMapper* fileMapper);
		void SetFileSize(unsigned long size);
	}
}