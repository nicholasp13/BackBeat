#include "bbpch.h"

#include "FileReader.h"
#include "TrackFactory.h"
namespace BackBeat {

	Track* TrackFactory::BuildTrack(std::string filePath)
	{
		AudioData* data = FileReader::CreateFile(filePath);
		if (data) {
			Track* track = new Track(data);
			return track;
		}
		return nullptr;
	}
}