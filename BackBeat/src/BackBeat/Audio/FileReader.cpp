#include "bbpch.h"

#include "FileReader.h"
#include "BackBeat/Core/Core.h"

#include <mmreg.h>
namespace BackBeat {

#define MP3 "ID3"
#define WAV "RIFF"

	bool FileReader::ReadFile(std::string filePath, tWAVEFORMATEX* props) 
	{
		char* header = new char[44];
		std::ifstream file;

		// TODO: Implement Function to get mp3/wav file header info
		file.open(filePath, std::ios::binary);
		if (file.is_open())
		{
			file.read(header, 44);
			file.close();

			char temp1 = header[3];
			char temp2 = header[4];
			header[3] = '\0';
			header[4] = '\0';
			
			if (std::strcmp(MP3, header) == 0)
			{
				header[3] = temp1;
				header[4] = temp2;
				BB_CORE_INFO("MP3 File opened");
				BB_CORE_INFO("HEADER: {0}", header);
				// ReadMP3Header(header, props);
				return true;
			}
			
			header[3] = temp1;
			if (std::strcmp(WAV, header) == 0)
			{
				header[4] = temp2;
				BB_CORE_INFO("WAV File opened");
				BB_CORE_INFO("HEADER: {0}", header);
				ReadWAVHeader(header, props);
				return true;
			}
		}
		BB_CORE_ERROR("FILE FAILED TO LOAD");
		return false;
	}


	/* TODO: CREATE INITIALIZE AFTER CREATING MP3 format reader */
	void FileReader::ReadMP3Header(char* header, tWAVEFORMATEX* props) {

	}

	void FileReader::ReadWAVHeader(char* header, tWAVEFORMATEX* props) {
		const int chunkID = 0;
		const int chunkSize = 4;
		const int format = 8;
		const int subChunk1ID = 12;
		const int subChunk1Size = 16;
		const int audioFormat = 20;
		const int numChannels = 22;
		const int sampleRate = 24;
		const int byteRate = 28;
		const int blockAlign = 32;
		const int bitsPerSample = 34;
		const int subChunk2ID = 36;
		const int subChunk2Size = 40;

		props->wFormatTag = (WORD)header[audioFormat];
		props->nChannels = (WORD)header[numChannels];
		props->nSamplesPerSec = (DWORD)header[sampleRate];
		props->nAvgBytesPerSec = (DWORD)header[byteRate];
		props->nBlockAlign = (WORD)header[blockAlign];
		props->wBitsPerSample = (WORD)header[bitsPerSample];
		props->cbSize = (WORD)0;
	}
}