#pragma once

#include <filesystem>
#include <string>
#include <vector>

namespace BackBeat {

	class FileManager
	{
	public:
		FileManager();
		~FileManager();

		std::string CreateSubDirectory(std::string dir);
		void DeleteSubDirectory(std::string dir);
		void MoveFileFullpath(std::string to, std::string from);

		bool SetWorkingDirectory(std::string dir);

		inline bool IsEmpty() { return m_WorkingDirectory.empty(); }
		inline bool Exists(std::string child) { return !Find(child).empty(); }
		inline std::string GetParentPath() { return m_WorkingDirectory.parent_path().string(); }
		inline std::string GetWorkingDirFullPath() { return m_WorkingDirectory.string(); }
		inline std::string GetWorkingDirName() { return m_WorkingDirectory.filename().string(); }
		
		std::string GetFilePath(std::string fileName);
		std::string GetSubDirPath(std::string subDir);
		std::vector< std::string > GetSubDirPaths();
		std::vector< std::string > GetFilePaths();
		std::vector< std::string > GetSubDirNames();
		std::vector< std::string > GetFileNames();

	private:
		std::filesystem::path m_WorkingDirectory;

	private:
		std::filesystem::path Find(std::string target);
	};

}