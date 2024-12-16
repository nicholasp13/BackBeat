#pragma once

#include "src/pugixml.hpp"

namespace BackBeat {

	class Serializable
	{
	public:
		virtual void WriteObject(pugi::xml_node* node) {}
		virtual void ReadObject(pugi::xml_node* node) {}

	};

}