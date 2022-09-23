#include "bbpch.h"

#include "FileReader.h"
#include "BackBeat/Core/Core.h"

namespace BackBeat {

#define MP3 "ID3"
#define WAV "RIFF"
#define BYTE 8

	HRESULT FileReader::GetHeader(int* position,  std::string filePath, tWAVEFORMATEX* props)
	{
		int headerSize = 44;
		char* header = new char[headerSize];
		std::ifstream file;

		// TODO: Implement Function to get mp3 file header info
		file.open(filePath, std::ios::binary);
		if (file.is_open())
		{
			file.read(header, headerSize);
			file.close();

			char temp1 = header[3];
			char temp2 = header[4];
			header[3] = '\0';
			header[4] = '\0';
			
			if (std::strcmp(MP3, header) == 0)
			{
				header[3] = temp1;
				header[4] = temp2;
				ReadMP3Header(header, props);
				BB_CORE_INFO("MP3 File opened");
				return S_OK;
			}
			
			header[3] = temp1;
			if (std::strcmp(WAV, header) == 0)
			{
				*position = 44;
				header[4] = temp2;
				ReadWAVHeader(header, props);
				BB_CORE_INFO("WAV File opened");
				return S_OK;
			}
		}
		*position = 0;
		BB_CORE_ERROR("FILE FAILED TO LOAD");
		return ERROR;
	}


	/* TODO: CREATE INITIALIZE AFTER CREATING MP3 format reader */
	void FileReader::ReadMP3Header(char* header, tWAVEFORMATEX* props) {

	}

	void FileReader::ReadWAVHeader(char* header, tWAVEFORMATEX* props) {
		
		// Indices of relevant tWAVEFORMATEX properties in standard WAV file headers 
		const int fileSize = 4;
		const int audioFormat = 20;
		const int numChannels = 22;
		const int sampleRate = 24;
		const int byteRate = 28;
		const int blockAlign = 32;
		const int bitsPerSample = 34;


		// Rearranges bytes from little Endian to big Endian for c++ standard units
		props->wFormatTag		= EndianConverterShort(header[audioFormat], header[audioFormat + 1]);
		
		props->nChannels		= EndianConverterShort(header[numChannels], header[numChannels + 1]);

		props->nSamplesPerSec	= EndianConverterLong(header[sampleRate], header[sampleRate + 1],
									header[sampleRate + 2], header[sampleRate + 3]);
		
		props->nAvgBytesPerSec	= EndianConverterLong(header[byteRate], header[byteRate + 1],
									header[byteRate + 2], header[byteRate + 3]);
		
		props->nBlockAlign		= EndianConverterShort(header[blockAlign], header[blockAlign + 1]);
		
		props->wBitsPerSample	= EndianConverterShort(header[bitsPerSample], header[bitsPerSample + 1]);
		
		props->cbSize			= EndianConverterLong(header[fileSize], header[fileSize + 1],
									header[fileSize + 2], header[fileSize + 3]);
	}

	unsigned short FileReader::EndianConverterShort(char num1, char num2)
	{
		unsigned short leftToRight = num1 & 0x00FF;
		unsigned short rightToLeft = num2 & 0x00FF << BYTE;
		return (leftToRight | rightToLeft);
	}

	unsigned long FileReader::EndianConverterLong(char num1, char num2,
													char num3, char num4)
	{
		unsigned long leftMost  = num1 & 0x000000FF;
		unsigned long left      = num2 & 0x000000FF << BYTE;
		unsigned long right     = num3 & 0x000000FF << 2 * BYTE;
		unsigned long rightMost = num4 & 0x000000FF << 3 * BYTE;
		return (left | leftMost | right | rightMost);
	}
}