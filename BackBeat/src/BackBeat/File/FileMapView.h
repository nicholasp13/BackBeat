#pragma once

// NOTE: Not currently used/implemented

// Interface for a class that is a view of virtual memory map of a file for quick reading and writing data to the file.
// This requires specific system API function calls so it will be defined in the Platform folder.
// Also closing and opening of the filemap view is handled by the lifetime of the object.

namespace BackBeat {

	class FileMapView
	{
		virtual void* GetBuffer() { return nullptr; }
		virtual unsigned int GetSize() { return 0; }

	};

}