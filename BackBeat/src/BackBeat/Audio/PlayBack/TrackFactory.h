#pragma once

#include "Track.h"
namespace BackBeat {

	class TrackFactory
	{
	public:
		static Track* BuildTrack(std::string filePath);

	};
}