#include "bbpch.h"

// TODO: Finish by adding pugixml
#include "src/pugixml.hpp"

#include "BackBeat/Core/Log.h"
#include "ProjectSerializer.h"
namespace BackBeat {


	ProjectSerializer::ProjectSerializer(std::shared_ptr<Project> project)
		: m_Project(project)
	{

	}

	ProjectSerializer::~ProjectSerializer()
	{

	}

	bool ProjectSerializer::Serialize(std::string filePath)
	{
		// Create pugi::xml_document
		auto doc = pugi::xml_document();

		ProjectConfig projectConfig = m_Project->GetConfig();

		auto app = doc.append_child(projectConfig.app);
		app.append_attribute("ProjectName") = projectConfig.name;
		app.append_attribute("WorkingDirectory") = projectConfig.projectDirectoryPath;

		auto objects = app.append_child("Objects");
		for (auto itr = projectConfig.objectList.begin(); itr != projectConfig.objectList.end(); itr++)
		{
			(*itr)->WriteObject(&objects);
		}

		// Write pugi::xml_document into filePath
		std::string testPath = "C:\\Dev\\Testing\\BackBeat\\test.xml"; // TODO: DELETE AND USE FILEPATH
		if (!doc.save_file(testPath.c_str()))
		{
			BB_CORE_ERROR("ERROR SAVING XML FILE");
			return false;
		}

		return true;
	}

	// TODO: Finish
	// NOTE: Object list deserialization is handled outside of ProjectSerializer
	bool ProjectSerializer::Deserialize(std::string filePath)
	{
		std::string testPath = "C:\\Dev\\Testing\\BackBeat\\test.xml"; // TODO: DELETE AND USE FILEPATH
		
		auto doc = pugi::xml_document();
		pugi::xml_parse_result result = doc.load_file(testPath.c_str());

		if (!result)
		{
			BB_CORE_ERROR("ERROR LOADING XML FILE");
			return false;
		}

		auto app = doc.first_child();
		m_Project->GetConfig().app = app.name();
		m_Project->GetConfig().name = app.attribute("ProjectName").as_string();
		m_Project->GetConfig().projectDirectoryPath = app.attribute("WorkingDirectory").as_string();
		m_Project->GetConfig().xmlFilePath = filePath;

		// TODO: Delete
		/**
		BB_CORE_TRACE("Project Config");
		BB_CORE_INFO("APP:               {0}", m_Project->GetConfig().app);
		BB_CORE_INFO("NAME:              {0}", m_Project->GetConfig().name);
		BB_CORE_INFO("PROJECT DIRECTORY: {0}", m_Project->GetConfig().projectDirectoryPath);
		BB_CORE_INFO("XML FILEPATH:      {0}", m_Project->GetConfig().xmlFilePath);
		BB_CORE_INFO("NUM OBJECTS:       {0}", m_Project->GetConfig().objectList.size());
		/**/
		return true;
	}

}