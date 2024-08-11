#include "bbpch.h"

#include <filesystem>

#include "Log.h"
#include "FileSystem.h"
namespace BackBeat {

	// Full filepath to temp dir: AppData\Local\BackBeat\Project\temp
	static std::string s_AppDataLocalDir = "";
	static const std::string s_BackBeatDir = "\\BackBeat";
	static std::string s_ProjectDir = "";
	static const std::string s_TempDir = "\\temp";

	void FileSystem::CreateBackBeatDir()
	{
		std::string fullPathDir = s_AppDataLocalDir.c_str() + s_BackBeatDir;
		auto filePath = std::filesystem::path(fullPathDir);
		std::filesystem::create_directory(filePath);
	}

	void FileSystem::CreateProjectDir()
	{
		CreateBackBeatDir();
		std::string fullPathDir = s_AppDataLocalDir.c_str() + s_BackBeatDir + s_ProjectDir;
		auto filePath = std::filesystem::path(fullPathDir);
		std::filesystem::create_directory(filePath);
	}

	void FileSystem::CreateTempDir()
	{
		CreateProjectDir();
		std::string fullPathDir = s_AppDataLocalDir.c_str() + s_BackBeatDir 
			+ s_ProjectDir + s_TempDir;
		auto filePath = std::filesystem::path(fullPathDir);
		std::filesystem::create_directory(filePath);
	}

	std::string FileSystem::GetAppDataLocalDir()
	{
		std::string filePath = s_AppDataLocalDir + "\\";
		return filePath;
	}

	std::string FileSystem::GetBackBeatDir()
	{
		std::string filePath = s_AppDataLocalDir.c_str() + s_BackBeatDir + "\\";
		return filePath;
	}

	std::string FileSystem::GetProjectDir()
	{
		std::string filePath = s_AppDataLocalDir.c_str() + s_BackBeatDir + s_ProjectDir + "\\";
		return filePath;
	}

	std::string FileSystem::GetTempDir()
	{
		std::string filePath = s_AppDataLocalDir.c_str() + s_BackBeatDir 
			+ s_ProjectDir + s_TempDir + "\\";
		return filePath;
	}

	void FileSystem::SetAppDataLocalDir(std::string dir)
	{
		s_AppDataLocalDir = dir;
	}

	// NOTE: All directrories should start with '\\' (this may need to be checked but it works fine regardless)
	void FileSystem::SetProjectDir(std::string dir)
	{
		s_ProjectDir = dir;
	}

}