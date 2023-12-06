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

}