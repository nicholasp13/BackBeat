#pragma once

#include <string>
#include <memory>
#include <vector>

#include "Serializable.h"
namespace BackBeat {

	struct ProjectConfig
	{
		std::string app = "";
		std::string name = "Untitled";
		std::string projectDirectoryPath = "";
		std::string xmlFilePath = "";
		std::vector< std::shared_ptr<Serializable> > objectList;
	};

	class Project
	{
	public:

		inline ProjectConfig& GetConfig() { return m_Config; }

		inline static std::shared_ptr<Project> GetActive() { return s_ActiveProject; }

		static std::shared_ptr<Project> New();
		static std::shared_ptr<Project> Load(std::string filePath);
		static bool SaveActive(std::string filePath);

	private:
		ProjectConfig m_Config;
		std::string m_ProjectDirectory;

		inline static std::shared_ptr<Project> s_ActiveProject;

	};
}