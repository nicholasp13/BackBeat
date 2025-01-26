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

	void FileManager::DeleteSubDirectory(std::string dir)
	{
		auto path = Find(dir);
		if (path.empty())
			return;

		std::filesystem::remove_all(path);
	}

	void FileManager::MoveFileFullpath(std::string to, std::string from)
	{
		std::filesystem::path target = to;
		std::filesystem::path source = from;

		if (!std::filesystem::exists(source))
			return;

		std::filesystem::rename(source, target);
	}

	bool FileManager::SetWorkingDirectory(std::string dir)
	{
		
		std::string directory = dir;
		
		if (directory.back() != '\\')
			directory += '\\';
		
		std::filesystem::path workingDir = directory;

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

	// NOTE: Should give only name of the file not the fullpath. Assumes directory is in current working directory
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