#pragma once

#include "Audio.h"
namespace BackBeat {

	class FileReader
	{
	public:
		FileReader() {};
		~FileReader() {};

		static std::string ReadFile(std::string fileName);
	};
}