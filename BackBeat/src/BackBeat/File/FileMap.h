#pragma once

#include "bbpch.h"

// Interface for a class that is a virtual memory map of a file for quick reading and writing data to the file.
// This requires specific system API function calls so it will be defined in the Platform folder.
// Also closing and opening of the filemap is handled by the lifetime of the object

namespace BackBeat {

	class FileMap
	{
	public:
		virtual bool ReadData(char* buffer, unsigned int size, unsigned int position) { return false; }
		virtual bool WriteData(char* buffer, unsigned int size, unsigned int position) { return false; }
		
	};

}