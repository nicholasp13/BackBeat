#pragma once

#include "Track.h"
namespace BackBeat {

	namespace TrackFactory
	{
		Track* BuildTrack(std::string filePath);
	}
}