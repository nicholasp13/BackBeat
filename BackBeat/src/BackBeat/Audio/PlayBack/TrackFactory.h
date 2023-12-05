#pragma once

#include "Track.h"
namespace BackBeat {

	class TrackFactory
	{
	public:
		TrackFactory();
		~TrackFactory();

		Track* BuildTrack(std::string filePath);

	private:
	};
}