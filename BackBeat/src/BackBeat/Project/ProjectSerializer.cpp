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

		if (!doc.save_file(filePath.c_str()))
		{
			BB_CORE_ERROR("ERROR SAVING XML FILE");
			return false;
		}

		return true;
	}

	// NOTE: Object list deserialization is handled outside of ProjectSerializer
	bool ProjectSerializer::Deserialize(std::string filePath)
	{	
		auto doc = pugi::xml_document();
		pugi::xml_parse_result result = doc.load_file(filePath.c_str());

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

		return true;
	}

}