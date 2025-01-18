#pragma once

#include <map>

#include "Platform/Windows/Windows.h"
#include "BackBeat/File/FileMapper.h"
#include "WindowsFileMap.h"
namespace BackBeat {

	class WindowsFileMapper : public FileMapper
	{
	public:
		WindowsFileMapper();
		~WindowsFileMapper();

		virtual std::shared_ptr<FileMap> MapFile(std::string filePath, unsigned long size) override;

	private:
		SYSTEM_INFO m_SystemInfo;
		DWORD m_SystemGranularity;

	};

}