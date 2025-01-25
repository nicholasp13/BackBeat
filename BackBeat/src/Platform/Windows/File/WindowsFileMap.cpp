#include "bbpch.h"

/**
* Windows API documents:
* https://learn.microsoft.com/en-us/windows/win32/memory/file-mapping
* https://learn.microsoft.com/en-us/windows/win32/memory/creating-a-view-within-a-file
*/

#include "BackBeat/Core/Log.h"
#include "BackBeat/Audio/Audio.h"
#include "WindowsFileMap.h"
namespace BackBeat {

	WindowsFileMap::WindowsFileMap(std::string filePath, WindowsMapHandles handles, DWORD size, DWORD sysGranularity)
		: 
		m_ToDelete(false),
		m_Mapped(true),
		m_Open(true),
		m_FileSize(size), 
		m_SystemGranularity(sysGranularity),
		m_FilePath(filePath), 
		m_Handles(handles)
	{

	}

	WindowsFileMap::~WindowsFileMap()
	{
		Unmap();
		if (m_ToDelete)
			std::filesystem::remove(m_FilePath);
	}

	bool WindowsFileMap::ReadData(char* buffer, unsigned int size, unsigned int position)
	{
		if (!m_Open || !m_Mapped)
			return false;
		if (position > m_FileSize)
			return false;

		unsigned int bytesToRead = size;
		if (position + bytesToRead > m_FileSize)
			bytesToRead = (unsigned int)(m_FileSize - (DWORD)position);

		DWORD fileMapStart = ((DWORD)position / m_SystemGranularity) * m_SystemGranularity;
		DWORD mapViewSize = ((DWORD)position % m_SystemGranularity) + (DWORD)bytesToRead;
		DWORD positionOffset = (DWORD)position - fileMapStart;
		LPVOID mapAddress = NULL;
		char* data = nullptr;

		mapAddress = MapViewOfFile(
			m_Handles.hMapFile,     // Windows handle to file map object
			FILE_MAP_ALL_ACCESS,    // read/write access
			0,                      // high order bytes of file offset (assumed 0 as file offset will probably not be that big)
			fileMapStart,           // low order bytes of file offset
			mapViewSize);           // number of bytes to map

		if (mapAddress == NULL)
		{
			BB_CORE_ERROR("Error occured during mapping file view in WindowsFileMap");
			BB_CORE_ERROR("Target file: {0}", m_FilePath.c_str());
			BB_CORE_ERROR("Error code:  {0}", GetLastError());
			return false;
		}

		data = (char*)mapAddress + positionOffset;
		Audio::CopyInputToOutput(data, buffer, bytesToRead);

		if (!UnmapViewOfFile(mapAddress))
		{
			BB_CORE_ERROR("Error occured during unmapping file view in WindowsFileMap");
			BB_CORE_ERROR("Target file: {0}", m_FilePath.c_str());
			BB_CORE_ERROR("Error code:  {0}", GetLastError());
			return false;
		}

		return true;
	}

	bool WindowsFileMap::WriteData(char* buffer, unsigned int size, unsigned int position)
	{
		if (!m_Open || !m_Mapped)
			return false;
		if (position > m_FileSize)
			return false;

		unsigned int bytesToWrite = size;
		if (position + bytesToWrite > m_FileSize)
			bytesToWrite = (unsigned int)(m_FileSize - (DWORD)position);

		DWORD fileMapStart = ((DWORD)position / m_SystemGranularity) * m_SystemGranularity;
		DWORD mapViewSize = ((DWORD)position % m_SystemGranularity) + (DWORD)bytesToWrite;
		DWORD positionOffset = (DWORD)position - fileMapStart;
		LPVOID mapAddress = NULL;
		char* data = nullptr;

		mapAddress = MapViewOfFile(
			m_Handles.hMapFile,     // Windows handle to file map object
			FILE_MAP_ALL_ACCESS,    // read/write access
			0,                      // high order bytes of file offset (assumed 0 as file offset will probably not be that big)
			fileMapStart,           // low order bytes of file offset
			mapViewSize);           // number of bytes to map

		if (mapAddress == NULL)
		{
			BB_CORE_ERROR("Error occured during mapping file view in WindowsFileMap");
			BB_CORE_ERROR("Target file: {0}", m_FilePath.c_str());
			BB_CORE_ERROR("Error code:  {0}", GetLastError());
			return false;
		}

		data = (char*)mapAddress + positionOffset;
		Audio::CopyInputToOutput(buffer, data, bytesToWrite);

		if (!UnmapViewOfFile(mapAddress))
		{
			BB_CORE_ERROR("Error occured during unmapping file view in WindowsFileMap");
			BB_CORE_ERROR("Target file: {0}", m_FilePath.c_str());
			BB_CORE_ERROR("Error code:  {0}", GetLastError());
			return false;
		}

		return true;
	}

	bool WindowsFileMap::Unmap()
	{
		bool flag = false;
		if (m_Mapped)
		{
			flag = CloseHandle(m_Handles.hMapFile);
			if (!flag)
			{
				BB_CORE_ERROR("Error occured during unmapping/closing map handle in WindowsFileMap");
				BB_CORE_ERROR("Target map file: {0}", m_FilePath.c_str());
				BB_CORE_ERROR("Error code:      {0}", GetLastError());
			}
			else
				m_Mapped = false;
		}

		if (m_Open)
		{
			flag = CloseHandle(m_Handles.hFile);
			if (!flag)
			{
				BB_CORE_ERROR("Error occured during unmapping/closing file handle in WindowsFileMap");
				BB_CORE_ERROR("Target file: {0}", m_FilePath.c_str());
				BB_CORE_ERROR("Error code:  {0}", GetLastError());
			}
			else
				m_Open = false;
		}

		return (!m_Mapped && !m_Open);
	}
}