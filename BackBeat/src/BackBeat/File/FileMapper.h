#pragma once

// Interface for a class that loads files into virtual memory using file mapping to quickly load data from files.
// This requires specific system API function calls so it will be defined in the Platform folder.

#include "bbpch.h"

#include "FileMap.h"
namespace BackBeat {

	class FileMapper
	{
	public:
		// Maps existing file to virtual memory using system API. Should return nullptr if the file does not exist
		virtual std::shared_ptr<FileMap> MapFile(std::string filePath, unsigned long size) { return nullptr; }
		// Creates new file and maps it into virtual memory using system API. Should return nullptr if file exists
		virtual std::shared_ptr<FileMap> CreateMappedFile(std::string filePath, unsigned long size) { return nullptr; }
	};

}