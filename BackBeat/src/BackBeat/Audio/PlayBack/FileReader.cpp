#include "bbpch.h"

#include "BackBeat/Audio/Audio.h"
#include "BackBeat/Core/Core.h"
#include "FileReader.h"
namespace BackBeat {

	HRESULT FileReader::CreateFile(std::string filePath, AudioData** audioData)
	{
		AudioProps props;
		unsigned long size = 0;
		int headerSize = 44;
		char* header = new char[headerSize];
		std::ifstream file;

		// TODO: Implement Function to get mp3 file header info
		//       Change the way filepath is taken? Use <filesystem> possibly
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
				ReadMP3Header(header, &props);
				BB_CORE_INFO("MP3 File opened {0}", filePath);
				return S_OK;
			}
			
			header[3] = temp1;
			if (std::strcmp(WAV, header) == 0)
			{
				props.bigEndian = 
				header[4] = temp2;
				ReadWAVHeader(header, &props);
				BB_CORE_INFO("WAV File opened {0}", filePath);
				if (props.numChannels > 2) return LOAD_FAILURE;

				// *audioData = new WAVData(filePath, &props, size);
				BB_CORE_INFO("WAV File created {0}", filePath);
				return S_OK;
			}
		}
		return LOAD_FAILURE;
	}


	/* TODO: CREATE INITIALIZE AFTER CREATING MP3 format reader */
	void FileReader::ReadMP3Header(char* header, AudioProps* props) 
	{

	}

	void FileReader::ReadWAVHeader(char* header, AudioProps* props)
	{
		
		// Indices of relevant properties in standard WAV file headers 
		// NOTE: Headers are always written in big endian
		const int fileSize		= 4;
		const int audioFormat	= 20;
		const int numChannels	= 22;
		const int sampleRate	= 24;
		const int byteRate		= 28;
		const int blockAlign	= 32;
		const int bitDepth = 34;

		if (Audio::IsBigEndian())
		{
			// TODO: CREATE WAY TO GET BIG ENDIAN INTO PROPS
			return;
		}
		// Rearranges bytes from little Endian to big Endian for this system's standard units
		props->format = Audio::EndianConverterShort(header[audioFormat], header[audioFormat + 1]);
		
		props->numChannels = Audio::EndianConverterShort(header[numChannels], header[numChannels + 1]);

		props->sampleRate = Audio::EndianConverterLong(header[sampleRate], header[sampleRate + 1],
									header[sampleRate + 2], header[sampleRate + 3]);
		props->byteRate	= Audio::EndianConverterLong(header[byteRate], header[byteRate + 1],
									header[byteRate + 2], header[byteRate + 3]);
		props->blockAlign= Audio::EndianConverterShort(header[blockAlign], header[blockAlign + 1]);
		
		props->bitDepth = Audio::EndianConverterShort(header[bitDepth], header[bitDepth + 1]);

		props->fileSize = Audio::EndianConverterLong(header[fileSize], header[fileSize + 1], header[fileSize + 2], header[fileSize + 3]);
	}
}