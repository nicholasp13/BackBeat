#pragma once

// Interface for a class that loads files into virtual memory using file mapping to quickly load data from files.
// This requires specific system API function calls so it will be defined in the Platform folder.
// Unmapping will be handled by the FileMap itself on deconstruction

#include "bbpch.h"

#include "FileMap.h"
namespace BackBeat {

	class FileMapper
	{
	public:
		virtual std::shared_ptr<FileMap> MapFile(std::string filePath, unsigned long size) { return nullptr; }
	};

}