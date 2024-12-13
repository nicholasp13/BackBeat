#include "bbpch.h"

#include "FileManager.h"
namespace BackBeat {

	FileManager::FileManager()
	{

	}

	FileManager::~FileManager()
	{

	}

	std::string FileManager::CreateSubDirectory(std::string dir)
	{
		if (IsEmpty())
			return std::string();

		std::string fullPathDir = m_WorkingDirectory.string() + dir;
		auto dirPath = std::filesystem::path(fullPathDir);
		std::filesystem::create_directory(dirPath);

		fullPathDir += "\\";

		return fullPathDir;
	}

	// TODO: FINISH
	bool FileManager::OpenSubDirectory(std::string subDir)
	{

		return false;
	}

	// TODO: FINISH
	bool FileManager::OpenParentDirectory()
	{

		return false;
	}

	// TODO: Check if "//" is at the end of the filepath and if not: make sure it is added.
	bool FileManager::SetWorkingDirectory(std::string dir)
	{
		std::filesystem::path workingDir = dir;
		
		if (workingDir.empty())
			return false;
		if (!std::filesystem::exists(workingDir))
			return false;
		if (!std::filesystem::is_directory(workingDir))
			return false;

		m_WorkingDirectory = workingDir;
		return true;
	}

	// NOTE: Should give only name of the file not the fullpath. Assumes file is in current working directory
	std::string FileManager::GetFilePath(std::string fileName)
	{
		auto targetFile = Find(fileName);

		if (targetFile.empty())
			return std::string();

		return targetFile.string();
	}

	// NOTE: Should give only name of the file not the fullpath. Assumes file is in current working directory
	std::string FileManager::GetSubDirPath(std::string subDir)
	{
		auto targetSubDir = Find(subDir);

		if (targetSubDir.empty() || !std::filesystem::is_directory(targetSubDir))
			return std::string();
		
		return targetSubDir.string();
	}

	std::vector< std::string > FileManager::GetSubDirPaths()
	{
		std::vector< std::string > subDirs;

		for (auto const& dirEntry : std::filesystem::directory_iterator{ m_WorkingDirectory })
		{
			if (dirEntry.is_directory())
			{
				subDirs.push_back(dirEntry.path().string());
			}
		}

		return subDirs;
	}

	std::vector< std::string > FileManager::GetFilePaths()
	{
		std::vector< std::string > files;

		for (auto const& dirEntry : std::filesystem::directory_iterator{ m_WorkingDirectory })
		{
			if (dirEntry.is_regular_file())
			{
				files.push_back(dirEntry.path().string());
			}
		}


		return files;
	}

	std::vector< std::string > FileManager::GetSubDirNames()
	{
		std::vector< std::string > subDirs;

		for (auto const& dirEntry : std::filesystem::directory_iterator{ m_WorkingDirectory })
		{
			if (dirEntry.is_directory())
			{
				subDirs.push_back(dirEntry.path().filename().string());
			}
		}

		return subDirs;
	}

	std::vector< std::string > FileManager::GetFileNames()
	{
		std::vector< std::string > files;

		for (auto const& dirEntry : std::filesystem::directory_iterator{ m_WorkingDirectory })
		{
			if (dirEntry.is_regular_file())
			{
				files.push_back(dirEntry.path().filename().string());
			}
		}


		return files;
	}

	std::filesystem::path FileManager::Find(std::string target)
	{

		std::filesystem::path targetPath = std::filesystem::path(m_WorkingDirectory.string() + target);

		if (!std::filesystem::exists(targetPath))
			targetPath.clear();

		return targetPath;
	}
}