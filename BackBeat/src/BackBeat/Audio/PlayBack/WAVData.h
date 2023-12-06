#pragma once

// NOTE: With current implementation this could just be a struct

#include "AudioData.h"
namespace BackBeat {

	class WAVData : public AudioData
	{
	public:
		WAVData(std::string filePath, std::string name, AudioProps props);
		~WAVData();
		
		virtual FileType GetFileType() { return FileType::WAV_FILE; }
		virtual std::string GetName() { return m_Name; }
		virtual std::string GetFilePath() { return m_FilePath; }
		virtual AudioProps GetProps() { return m_Props; }
		virtual UINT32 GetSize() { return m_Props.fileSize; }
		virtual unsigned int GetZero() { return m_Zero; }
		virtual unsigned int GetDataSize() { return m_DataSize; }

		virtual void SetZero(unsigned int position) { m_Zero = position; }
		virtual void SetDataSize(unsigned int size) { m_DataSize = size; }

	private:
		unsigned int m_Zero;
		unsigned int m_DataSize;
		std::string m_FilePath;
		std::string m_Name;
		AudioProps m_Props;

	};
}