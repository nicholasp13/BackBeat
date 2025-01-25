#pragma once

#include "Windows.h"

#include "BackBeat/File/FileMap.h"
namespace BackBeat {

	struct WindowsMapHandles
	{
		HANDLE hFile = nullptr;
		HANDLE hMapFile = nullptr;
	};

	class WindowsFileMap : public FileMap
	{
	public: 
		WindowsFileMap(std::string filePath, WindowsMapHandles handles, DWORD size, DWORD sysGranularity);
		~WindowsFileMap();
		
		virtual bool ReadData(char* buffer, unsigned int size, unsigned int position) override;
		virtual bool WriteData(char* buffer, unsigned int size, unsigned int position) override;
		
		inline virtual void SetToDelete() override { m_ToDelete = true; }

	private:
		bool m_ToDelete;
		bool m_Mapped;
		bool m_Open;
		DWORD m_FileSize;
		DWORD m_SystemGranularity;
		std::string m_FilePath;
		WindowsMapHandles m_Handles;

	private:
		bool Unmap();

	};

}