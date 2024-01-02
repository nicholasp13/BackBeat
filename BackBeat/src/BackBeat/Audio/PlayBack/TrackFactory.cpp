#include "bbpch.h"

#include "FileReader.h"
#include "TrackFactory.h"
namespace BackBeat {

	Track* TrackFactory::BuildTrack(std::string filePath)
	{
		AudioInfo info = FileReader::CreateFile(filePath);
		if (info.type != none) {
			Track* track = new Track(info);
			return track;
		}

		return nullptr;
	}
}