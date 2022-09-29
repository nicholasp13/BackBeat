#pragma once

#include "AudioData.h"

namespace BackBeat {

	class WAVData : public AudioData
	{
	public:
		WAVData(std::string filePath,tWAVEFORMATEX* props,int size);
		~WAVData();

		virtual HRESULT LoadBuffer(UINT32 framesAvailable, BYTE* buffer, UINT32* position);
		virtual FileType GetFileType();
		virtual tWAVEFORMATEX* GetProperties();
		
	private:
		unsigned long m_Size = 0;
		std::ifstream m_Data;
		std::string m_FilePath = "";
		tWAVEFORMATEX m_Props;

	};
}