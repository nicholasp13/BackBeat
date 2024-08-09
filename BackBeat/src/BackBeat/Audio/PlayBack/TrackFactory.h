#pragma once

#include "Track.h"
namespace BackBeat {

	namespace TrackFactory
	{
		std::shared_ptr<Track> BuildTrack(std::string filePath);
	}
}