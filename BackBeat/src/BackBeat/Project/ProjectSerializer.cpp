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

	// TODO: Finish 
	bool ProjectSerializer::Serialize(std::string filePath)
	{
		// Create pugi::xml_document
		auto doc = pugi::xml_document();

		// Serialize Project Configs
		// -> Serializes name
		// -> Serializes filePath to working Project directory
		// -> Serializes the list of objects
		// -> -> Object serializes itself onto pugi::xml_document (by passing a pointer to the document from this function
		//                                                         and the objects own pointer into a static Object::Serialize()
		//                                                         function
		{
			ProjectConfig projectConfig = m_Project->GetConfig();
			BB_CORE_TRACE("Project Config");
			BB_CORE_INFO("APP:               {0}", projectConfig.app);
			BB_CORE_INFO("NAME:              {0}", projectConfig.name);
			BB_CORE_INFO("PROJECT DIRECTORY: {0}", projectConfig.projectDirectoryPath);
			BB_CORE_INFO("XML FILEPATH:      {0}", projectConfig.xmlFilePath);
			BB_CORE_INFO("NUM OBJECTS:       {0}", projectConfig.objectList.size());

			auto app = doc.append_child(projectConfig.app);

			auto name = doc.append_child("Name");
			name.append_attribute(projectConfig.name);

			auto directory = doc.append_child("Working Directory");
			directory.append_attribute(projectConfig.projectDirectoryPath);

			for (auto itr = projectConfig.objectList.begin(); itr != projectConfig.objectList.end(); itr++)
			{
				// TODO: Finish
			}
		}


		// Write pugi::xml_document into filePath
		return false;
	}

	// TODO: Finish
	bool ProjectSerializer::Deserialize(std::string filePath)
	{

		return false;
	}

}