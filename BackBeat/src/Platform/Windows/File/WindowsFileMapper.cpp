#include "bbpch.h"

/**
* Windows API documents:
* https://learn.microsoft.com/en-us/windows/win32/memory/file-mapping
*/
#include "WindowsFileMapper.h"
namespace BackBeat {

	WindowsFileMapper::WindowsFileMapper()
	{
		GetSystemInfo(&m_SystemInfo);
		m_SystemGranularity = m_SystemInfo.dwAllocationGranularity;
	}
	WindowsFileMapper::~WindowsFileMapper()
	{
		
	}

	std::shared_ptr<FileMap> WindowsFileMapper::MapFile(std::string filePath, unsigned long size)
	{
		LPCSTR lpcTheFile = filePath.c_str();  // Windows char pointer to the file path
		HANDLE hMapFile      = nullptr;        // handle for the file's memory-mapped region
		HANDLE hFile         = nullptr;        // the file handle
		BOOL bFlag           = false;          // a result holder
		DWORD dBytesWritten  = 0;              // number of bytes written
		DWORD dwFileMapSize  = size;           // size of the file mapping
		DWORD error          = 0;              // Where error codes will be stored or NO_ERROR if none occured

		hFile = CreateFileA(lpcTheFile,        // File name as in the full path
			GENERIC_READ | GENERIC_WRITE,      // Desired access
			0,                                 // Shared mode, 0 indicates that it is not in shared mode and the 
			                                   // file must be closed before it can be shared or opened by another process
			NULL,                              // Security attributes, NULL indicates default security, look into
			                                   // https://learn.microsoft.com/en-us/windows/win32/api/wtypesbase/ns-wtypesbase-security_attributes
			                                   // for more info
			OPEN_EXISTING,                     // Action taken on whether the file exists or not, OPEN_EXISTING means the file
			                                   // must exist for CreateFile to be successful. Last error set to ERROR_FILE_NOT_FOUND (2) if not.
			FILE_ATTRIBUTE_NORMAL,             // File attributes, FILE_ATTRIBUTE_NORMAL most commmon default values
			NULL);                             // Handle to a template file, optional, NULL if not using

		if (hFile == INVALID_HANDLE_VALUE)
		{
			BB_CORE_ERROR("hFile is NULL");
			BB_CORE_ERROR("Target file is %s", lpcTheFile);
			return nullptr;
		}

		error = GetLastError();
		if (error == ERROR_FILE_NOT_FOUND)
		{
			BB_CORE_ERROR("File not found");
			BB_CORE_ERROR("Target file is %s", lpcTheFile);
			return nullptr;
		}
		else if (error != NO_ERROR)
		{
			BB_CORE_ERROR("Error occured in Windows CreateFileA");
			BB_CORE_ERROR("Error code: {0}", error);
			return nullptr;
		}

		hMapFile = CreateFileMappingA(hFile,         // current file handle
			NULL,                                    // default security
			PAGE_READWRITE,                          // read/write permission
			0,                                       // size of mapping object, high
			dwFileMapSize,                           // size of mapping object, low
			NULL);                                   // name of mapping object

		error = GetLastError();
		if (hMapFile == NULL || error != NO_ERROR)
		{
			BB_CORE_ERROR("Error occured in Windows CreateFileMappingA");
			BB_CORE_ERROR("Error code: {0}", error);
			return nullptr;
		}

		WindowsMapHandles handles;
		handles.hFile = hFile;
		handles.hMapFile = hMapFile;
		auto fileMap = std::make_shared<WindowsFileMap>(filePath, handles, (DWORD)size, m_SystemGranularity);

		return fileMap;
	}

	std::shared_ptr<FileMap> WindowsFileMapper::CreateMappedFile(std::string filePath, unsigned long size)
	{
		LPCSTR lpcTheFile = filePath.c_str();  // Windows char pointer to the file path
		HANDLE hMapFile = nullptr;             // handle for the file's memory-mapped region
		HANDLE hFile = nullptr;                // the file handle
		BOOL bFlag = false;                    // a result holder
		DWORD dBytesWritten = 0;               // number of bytes written
		DWORD dwFileMapSize = size;            // size of the file mapping
		DWORD error = 0;                       // Where error codes will be stored or NO_ERROR if none occured

		hFile = CreateFileA(lpcTheFile,        // File name as in the full path
			GENERIC_READ | GENERIC_WRITE,      // Desired access
			0,                                 // Shared mode, 0 indicates that it is not in shared mode and the 
			                                   // file must be closed before it can be shared or opened by another process
			NULL,                              // Security attributes, NULL indicates default security, look into
			                                   // https://learn.microsoft.com/en-us/windows/win32/api/wtypesbase/ns-wtypesbase-security_attributes
			                                   // for more info
			CREATE_NEW,                        // Action taken on whether the file exists or not, CREATE_NEW means the file
			                                   // must NOT exist for CreateFile to be successful. Last error set to ERROR_FILE_EXISTS (80) if not.
			FILE_ATTRIBUTE_NORMAL,             // File attributes, FILE_ATTRIBUTE_NORMAL most commmon default values
			NULL);                             // Handle to a template file, optional, NULL if not using

		if (hFile == INVALID_HANDLE_VALUE)
		{
			BB_CORE_ERROR("hFile is NULL");
			BB_CORE_ERROR("Target file is %s", lpcTheFile);
			return nullptr;
		}

		error = GetLastError();
		if (error == ERROR_FILE_EXISTS)
		{
			BB_CORE_ERROR("File exists. Must be new file for file to be created");
			BB_CORE_ERROR("Target file is %s", lpcTheFile);
			return nullptr;
		}
		else if (error != NO_ERROR)
		{
			BB_CORE_ERROR("Error occured in Windows CreateFileA");
			BB_CORE_ERROR("Error code: {0}", error);
			return nullptr;
		}

		hMapFile = CreateFileMappingA(hFile,         // current file handle
			NULL,                                    // default security
			PAGE_READWRITE,                          // read/write permission
			0,                                       // size of mapping object, high
			dwFileMapSize,                           // size of mapping object, low
			NULL);                                   // name of mapping object

		error = GetLastError();
		if (hMapFile == NULL || error != NO_ERROR)
		{
			BB_CORE_ERROR("Error occured in Windows CreateMappedFile");
			BB_CORE_ERROR("Error code: {0}", error);
			return nullptr;
		}

		WindowsMapHandles handles;
		handles.hFile = hFile;
		handles.hMapFile = hMapFile;
		auto fileMap = std::make_shared<WindowsFileMap>(filePath, handles, (DWORD)size, m_SystemGranularity);
		return fileMap;
	}

}