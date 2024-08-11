#include "bbpch.h"

#include "RecorderBuilder.h"
namespace BackBeat {

	static std::string s_FileDirectory = "";
	static std::string s_FileType = "";

	std::shared_ptr<AudioRecorder> RecorderBuilder::BuildRecorder(UUID id, AudioProps props) 
	{
		std::string filePath = GetFilePath(id);
		return std::make_shared<AudioRecorder>(filePath, props);
	}

	std::string RecorderBuilder::GetFilePath(UUID id)
	{
		std::string fileName = id.ToString();
		std::string filePath = s_FileDirectory.c_str() + fileName + s_FileType.c_str();
		return filePath;
	}

	void RecorderBuilder::SetFileDirectory(std::string dir)
	{
		s_FileDirectory = dir;
	}

	void RecorderBuilder::SetFileType(std::string type)
	{
		s_FileType = type;
	}

}