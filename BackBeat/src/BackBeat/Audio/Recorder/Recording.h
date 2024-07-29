#pragma once

#include "BackBeat/Audio/Audio.h" 
namespace BackBeat {

	class Recording
	{
	public:
		Recording(std::string filePath, AudioProps props);
		~Recording();

		bool SaveWAV(std::string filePath);
		void Record(char* data, unsigned int numFrames);
		void Reset();
		void Reset(AudioProps props);
		void Reset(std::string filePath, AudioProps props);
		void GetData(char* buffer, unsigned long numBytes, unsigned long position);
		TimeMinSec GetLengthMinSecs();
		
		inline AudioProps GetProps() { return m_Props; }
		inline unsigned long GetSize() { return m_Size; }

	private:
		unsigned long m_Size;
		std::string m_TempPath;
		AudioProps m_Props;

		void Delete();
		void CreateTempFile();
	};

}