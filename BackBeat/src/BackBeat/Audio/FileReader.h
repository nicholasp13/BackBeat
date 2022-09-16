#pragma once

namespace BackBeat {

	class FileReader
	{
	public:
		FileReader() {};
		~FileReader() {};

		static bool ReadFile(std::string fileName, tWAVEFORMATEX* props);
	};
}