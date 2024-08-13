#pragma once

#include <string>

namespace BackBeat {

	namespace FileSystem
	{
		void CreateBackBeatDir();
		void CreateProjectDir();
		void CreateTempDir();
		std::string GetAppDataLocalDir();
		std::string GetBackBeatDir();
		std::string GetProjectDir();
		std::string GetTempDir();
		void SetAppDataLocalDir(std::string dir);
		void SetProjectDir(std::string dir);
	}

}