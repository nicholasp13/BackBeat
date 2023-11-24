#pragma once

#include "AudioData.h"
namespace BackBeat {

	class WAVData : public AudioData
	{
	public:
		WAVData(std::string filePath,tWAVEFORMATEX* props,int size);
		~WAVData();

		virtual HRESULT LoadBuffer(UINT32 bufferSize, BYTE* buffer, UINT32* position, bool* loading);
		virtual FileType GetFileType();
		virtual tWAVEFORMATEX* GetProperties();
		virtual UINT32 GetSize();

		// TODO: GET OTHER DATA FROM SUBCHUNK I.E. artist and song name if available
	private:
		unsigned long m_Size = 0;
		std::string m_FilePath = "";
		tWAVEFORMATEX m_Props;

	};
}