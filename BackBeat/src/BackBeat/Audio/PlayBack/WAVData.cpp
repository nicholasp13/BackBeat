#include "bbpch.h"

#include "BackBeat/Audio/Audio.h"
#include "BackBeat/Core/Core.h"
#include "WAVData.h"
namespace BackBeat {

	WAVData::WAVData(std::string filePath, std::string name, AudioProps props)
		: 
		m_Zero(0), 
		m_DataSize(0), 
		m_FilePath(filePath), 
		m_Name(name), 
		m_Props(props)
	{
		
	}

	WAVData::~WAVData()
	{
	}

	bool WAVData::LoadBuffer(unsigned int byteTotal, byte* buffer, unsigned int* position, bool* loading) 
	{
		char data;
		std::ifstream file(m_FilePath, std::ios::binary);

		if (m_Props.bigEndian == Audio::IsBigEndian())
		{
			for (UINT32 i = 0; i < byteTotal; i++)
			{
				if (*position + i >= m_Props.fileSize || !(*loading))
				{
					file.close();
					*position = *position + i;
					return true;
				}
				file.seekg(*position + i);
				file.get(data);
				buffer[i] = data;
			}
		}
		else
		{
			// TODO: Implement changing endianness using 
		}
		file.close();
		*position = *position + byteTotal;
		return false;
	}

}