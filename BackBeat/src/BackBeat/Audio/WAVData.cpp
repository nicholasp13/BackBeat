#include "bbpch.h"

#include "WAVData.h"
#include "BackBeat/Core/Core.h"

#include <Audioclient.h>

namespace BackBeat {

	WAVData::WAVData(std::string filePath, tWAVEFORMATEX* props, int size)
		: m_FilePath(filePath), m_Props(*props), m_Size(size)
	{
		BB_CORE_INFO("FILE PROPERTIES");
		BB_CORE_TRACE("Audio Format: {0}", m_Props.wFormatTag);
		BB_CORE_TRACE("Number of Channels: {0}", m_Props.nChannels);
		BB_CORE_TRACE("Sample Rate: {0}", m_Props.nSamplesPerSec);
		BB_CORE_TRACE("Byte Rate: {0}", m_Props.nAvgBytesPerSec);
		BB_CORE_TRACE("Block Align: {0}", m_Props.nBlockAlign);
		BB_CORE_TRACE("Bits per Sample: {0}", m_Props.wBitsPerSample);
		BB_CORE_TRACE("File Size: {0}", m_Size);
	}

	WAVData::~WAVData()
	{

	}

	HRESULT WAVData::FormatStream(tWAVEFORMATEX* deviceProps) 
	{

		return S_OK;
	}

	// TODO: CHANGE PARAMETERS OF FUNCTION TO ALLOW FOR ANOTHER CLASS TO CHANGE/FORMAT DATA TO BUFFER
	HRESULT WAVData::LoadBuffer(UINT32 framesAvailable, BYTE* buffer, unsigned int* position, DWORD* flags) 
	{
		char data;
		std::ifstream m_Data(m_FilePath, std::ios::binary);

		// TODO: CHANGE IMPLEMENTATION TO MATCH ENDIANESS
		for (int i = 0; i < (int)framesAvailable; i++)
		{
			m_Data.seekg(*position + i);
			m_Data.get(data);
			buffer[framesAvailable - i - 1] = data;
			if (*position + i >= m_Size)
			{
				*flags = AUDCLNT_BUFFERFLAGS_SILENT;
				m_Data.close();
				return S_OK;
			}
		}
		m_Data.close();
		*position = *position + framesAvailable;
		return S_OK;
	}

	FileType WAVData::GetFileType() { return FileType::WAV; }

}