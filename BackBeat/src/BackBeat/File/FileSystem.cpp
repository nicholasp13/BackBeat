#include "bbpch.h"

#include <filesystem>

#include "Log.h"
#include "FileSystem.h"
namespace BackBeat {

	// Full filepath to app dir:     Users\User\AppData\Local\BackBeat\App
	// Full filepath to configs dir: Users\User\AppData\Local\BackBeat\configs
	// Full filepath to temp dir:    Users\User\AppData\Local\BackBeat\temp
	static std::string s_AppDataLocalDir = "";
	static const std::string s_BackBeatDir = "\\BackBeat";
	static std::string s_AppDir = "";
	static const std::string s_ConfigsDir = "\\configs";
	static const std::string s_TempDir = "\\temp";

	void FileSystem::CreateBackBeatDir()
	{
		std::string fullPathDir = s_AppDataLocalDir.c_str() + s_BackBeatDir;
		auto filePath = std::filesystem::path(fullPathDir);
		std::filesystem::create_directory(filePath);
	}

	void FileSystem::CreateAppDir()
	{
		CreateBackBeatDir();
		std::string fullPathDir = s_AppDataLocalDir.c_str() + s_BackBeatDir + s_AppDir;
		auto filePath = std::filesystem::path(fullPathDir);
		std::filesystem::create_directory(filePath);
	}

	void FileSystem::CreateConfigsDir()
	{
		CreateAppDir();
		std::string fullPathDir = s_AppDataLocalDir.c_str() + s_BackBeatDir + s_TempDir;
		auto filePath = std::filesystem::path(fullPathDir);
		std::filesystem::create_directory(filePath);
	}

	void FileSystem::CreateTempDir()
	{
		CreateAppDir();
		std::string fullPathDir = s_AppDataLocalDir.c_str() + s_BackBeatDir + s_ConfigsDir;
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

	std::string FileSystem::GetAppDir()
	{
		std::string filePath = s_AppDataLocalDir.c_str() + s_BackBeatDir + s_AppDir + "\\";
		return filePath;
	}
 
	std::string FileSystem::GetTempDir()
	{
		std::string filePath = s_AppDataLocalDir.c_str() + s_BackBeatDir + s_TempDir + "\\";
		return filePath;
	}

	// NOTE: All directrories should start with '\\' (this may need to be checked but it works fine regardless)
	void FileSystem::SetAppDataLocalDir(std::string dir)
	{
		s_AppDataLocalDir = dir;
	}

	// NOTE: All directrories should start with '\\' (this may need to be checked but it works fine regardless)
	void FileSystem::SetAppDir(std::string dir)
	{
		s_AppDir = dir;
	}

	// NOTE: All directrories should start with '\\' (this may need to be checked but it works fine regardless)
	std::string AddAppSubDir(std::string subDir)
	{
		std::string filePath = s_AppDataLocalDir.c_str() + s_BackBeatDir + 
			s_AppDir + subDir + "\\";
		std::filesystem::create_directory(filePath);
		return filePath;
	}
}