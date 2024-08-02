#include "bbpch.h"

#include "RecorderBuilder.h"
namespace BackBeat {

	std::shared_ptr<AudioRecorder> RecorderBuilder::BuildRecorder(UUID id, AudioProps props) 
	{
		// NOTE: Empty strings for now but will possibly change in the future whem it's decided where user files will go
		const std::string fileDirectory = "";
		const std::string fileType = "";
		std::string fileName = std::to_string(unsigned long long(id));
		std::string filePath = fileDirectory + fileName + fileType;
		return std::make_shared<AudioRecorder>(filePath, props);
	}

}