#pragma once

#include "BackBeat/Core/UUID.h"
#include "BackBeat/Core/FileSystem.h"
#include "Track.h"
namespace BackBeat {

	namespace TrackFactory
	{
		std::shared_ptr<Track> BuildTrack(std::string filePath);
		std::shared_ptr<Track> BuildTempTrack(UUID id, AudioProps props);
	}
}