#include "bbpch.h"

// TODO: Include ProjectSerializer then finish implementing
#include "ProjectSerializer.h"

#include "Project.h"
namespace BackBeat {

	std::shared_ptr<Project> Project::New()
	{
		s_ActiveProject = std::make_shared<Project>();
		return s_ActiveProject;
	}

	std::shared_ptr<Project> Project::Load(std::string filePath)
	{
		auto project = std::shared_ptr<Project>();
		ProjectSerializer serializer(project);

		if (serializer.Deserialize(filePath))
		{
			s_ActiveProject = project;
			return s_ActiveProject;
		}

		return nullptr;
	}

	bool Project::SaveActive(std::string filePath)
	{
		ProjectSerializer serializer(s_ActiveProject);

		if (serializer.Serialize(filePath))
		{
			return true;
		}

		return false;
	}

}