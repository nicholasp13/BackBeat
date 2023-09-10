#include "bbpch.h"

#include "BackBeat/Audio/Audio.h"
#include "WAVData.h"
#include "BackBeat/Core/Core.h"

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

	// README
	// MIGHT NEED TO CREATE A CHECK/SEPERATE LOOP FOR DIFFERENT ENDIANNESS
	HRESULT WAVData::LoadBuffer(UINT32 byteTotal, BYTE* buffer, UINT32* position, bool* loading) 
	{
		char data;
		std::ifstream file(m_FilePath, std::ios::binary);

		for (UINT32 i = 0; i < byteTotal; i++)
		{
			if (*position + i >= m_Size || !(*loading))
			{
				file.close();
				*position = *position + i;
				return S_OK;
			}
			file.seekg(*position + i);
			file.get(data);
			buffer[i] = data;	
		}
		file.close();
		*position = *position + byteTotal;
		return S_OK;
	}

	FileType WAVData::GetFileType() { return FileType::WAV_FILE; }

	tWAVEFORMATEX* WAVData::GetProperties() { return &m_Props;  }

	UINT32 WAVData::GetSize() {	return m_Size; }

}