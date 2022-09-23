#include "bbpch.h"

#include "WAVData.h"

namespace BackBeat {

	WAVData::WAVData(std::string filePath, tWAVEFORMATEX* props)
		: m_FilePath(filePath), m_Props(*props)
	{
		CreateFile();
	}

	WAVData::~WAVData()
	{

	}

	HRESULT AudioData::FormatStream(tWAVEFORMATEX* deviceProps) 
	{
		return S_OK;
	}

	HRESULT AudioData::LoadBuffer(UINT32 framesAvaiable, BYTE* buffer, DWORD* flag) 
	{
		return S_OK;
	}

	FileType AudioData::GetFileType() { return FileType::WAV; }

	// TODO: Finish this function
	HRESULT WAVData::CreateFile()
	{
		return S_OK;
	}
}