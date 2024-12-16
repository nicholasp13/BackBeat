#pragma once

#include "src/pugixml.hpp"
#include "Project.h"

namespace BackBeat {

	class ProjectSerializer
	{
	public:
		ProjectSerializer(std::shared_ptr<Project> project);
		~ProjectSerializer();

		bool Serialize(std::string filePath);
		bool Deserialize(std::string filePath);

	private:
		std::shared_ptr<Project> m_Project;

	};
}