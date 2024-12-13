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

	private:
		template<typename T>
		void Serialize(const pugi::xml_document& xmlDoc, const T& object)
		{
			T::Serialize(xmlDoc, object);
		}

		template<typename T>
		void Deserialize(const pugi::xml_document& xmlDoc, const T& object)
		{
			T::Deserialize(xmlDoc, object);
		}

	};
}