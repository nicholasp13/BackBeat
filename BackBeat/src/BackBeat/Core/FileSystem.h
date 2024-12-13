#pragma once

#include <string>

namespace BackBeat {

	namespace FileSystem
	{
		void CreateBackBeatDir();
		void CreateAppDir();
		void CreateConfigsDir();
		void CreateTempDir();

		std::string GetAppDataLocalDir();
		std::string GetBackBeatDir();
		std::string GetAppDir();
		std::string GetTempDir();

		void SetAppDataLocalDir(std::string dir);
		void SetAppDir(std::string dir);

		std::string AddAppSubDir(std::string subDir);
	}

}