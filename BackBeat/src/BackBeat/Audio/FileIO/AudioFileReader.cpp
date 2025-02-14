#include "bbpch.h"

#include "BackBeat/Audio/Audio.h"
#include "BackBeat/Core/Core.h"
#include "AudioFileReader.h"
namespace BackBeat {

	AudioInfo AudioFileReader::ReadFile(std::string filePath)
	{
		unsigned int size = 0;
		char header[Audio::WAVHeaderSize] = {};
		std::ifstream file;

		file.open(filePath, std::ios::binary);
		if (file.is_open())
		{
			file.read(header, Audio::WAVHeaderSize);
			file.close();

			const unsigned int headerPosition = 3;
			const unsigned int sampleHeaderPosition = 6;
			char temp1 = header[headerPosition];
			char temp2 = header[headerPosition + 1];
			char temp3 = header[sampleHeaderPosition];
			header[headerPosition] = '\0';
			header[headerPosition + 1] = '\0';
			header[sampleHeaderPosition] = '\0';
			
			if (std::strcmp(Audio::MP3, header) == 0)
			{
				BB_CORE_ERROR("MP3 File found {0} \nMP3 decoding not yet implemented", filePath);
				return ReadMP3Header(filePath, 0);
			}
			
			if (std::strcmp(Audio::WAV, header) == 0)
			{
				header[headerPosition] = temp1;
				header[headerPosition + 1] = temp2;
				header[sampleHeaderPosition] = temp3;

				if (Audio::IsBigEndian()) 
				{
					size = *reinterpret_cast<unsigned int*>(header + headerPosition + 1);
				}
				else 
				{
					size = Audio::EndianConverterLong(header[headerPosition + 1], header[headerPosition + 2],
						header[headerPosition + 3], header[headerPosition + 4]);
				}

				// Some applications I've found do not properly set the size parameters in their WAV headers
				// So this is a fail safe
				if (size == (unsigned int)0xFFFFFFFF)
					size = (unsigned int)std::filesystem::file_size(filePath);

				return ReadHeader(filePath, size, FileType::wav);
			}

			header[headerPosition] = temp1;
			header[headerPosition + 1] = temp2;
			if (std::strcmp(Audio::SAMPLE, header) == 0)
			{
				header[sampleHeaderPosition] = temp3;
				if (Audio::IsBigEndian()) 
				{
					// size = *reinterpret_cast<unsigned long int*>(header + sampleHeaderPosition);
					size = Audio::EndianConverterLong(header[sampleHeaderPosition + 3], header[sampleHeaderPosition + 2],
						header[sampleHeaderPosition + 1], header[sampleHeaderPosition ]);
				}
				else 
				{
					size = Audio::EndianConverterLong(header[sampleHeaderPosition], header[sampleHeaderPosition + 1],
						header[sampleHeaderPosition + 2], header[sampleHeaderPosition + 3]);
				}
				return ReadHeader(filePath, size, FileType::sample);
			}
		}
		return AudioInfo();
	}

	bool AudioFileReader::ReadAudioFileData(
		std::string filePath, 
		char* data, 
		unsigned int position, 
		unsigned int numBytes)
	{
		std::ifstream file;
		file.open(filePath, std::ios::binary);
		if (file.is_open())
		{
			file.seekg(position);
			file.read(data, numBytes);
			file.close();
			return true;
		}

		return false;
	}

	AudioInfo AudioFileReader::ReadMP3Header(std::string filePath, unsigned int size)
	{
		return AudioInfo();
	}

	// Gets relevant info from headers on WAV and SAMPLE files and creates WavData pointer
	// based on that info
	// NOTE: WAV file headers are standardized and this function will fail if the WAV file
	//       is not properly formatted
	AudioInfo AudioFileReader::ReadHeader(std::string filePath, unsigned int size, FileType type)
	{
		char data;
		std::ifstream file;
		AudioProps props = AudioProps();
		AudioInfo info = AudioInfo();
		info.type = type;
		props.fileSize = size;
		file.open(filePath, std::ios::binary);
		
		if (!file.is_open())
			return info;

		if (info.type == FileType::wav)
		{
			file.seekg(3);
			file.get(data);
			props.bigEndian = (data == 'X');
		}
		else if (info.type == FileType::sample)
		{
			props.bigEndian = false;
		}
		else
		{
			file.close();
			return info;
		}

		int fmtPosition = -1;
		char fmt[4] = {};
		fmt[3] = '\0';
		for (unsigned int i = 0; i < size; i++)
		{
			file.seekg(i);
			file.get(data);

			if (data == 'f' && i + Audio::WAVFormatSize < size) 
			{
				file.seekg(i);
				file.read(fmt, 3);
				if (std::strcmp("fmt", fmt) == 0) 
				{
					fmtPosition = i;
					break;
				}
			}
		}

		if (fmtPosition < 0)
		{
			file.close();
			return info;
		}

		file.seekg(fmtPosition);
		char fileProps[Audio::WAVFormatSize];
		file.read(fileProps, Audio::WAVFormatSize);

		// Indices of relevant properties in standard WAV file headers 
		// All written in little endian
		const int fmtSize		= 4;
		const int audioFormat	= 8;
		const int numChannels	= 10;
		const int sampleRate	= 12;
		const int byteRate		= 16;
		const int blockAlign	= 20;
		const int bitDepth = 22;

		if (Audio::IsBigEndian())
		{
			props.format = (unsigned short)fileProps[audioFormat];
			props.numChannels = (unsigned short)fileProps[numChannels];
			props.sampleRate = (unsigned long)fileProps[sampleRate];
			props.byteRate = (unsigned long)fileProps[byteRate];
			props.blockAlign = (unsigned short)fileProps[blockAlign];
			props.bitDepth = (unsigned short)fileProps[bitDepth];
		}
		else 
		{
			props.format = Audio::EndianConverterShort(fileProps[audioFormat], fileProps[audioFormat + 1]);
			props.numChannels = Audio::EndianConverterShort(fileProps[numChannels], fileProps[numChannels + 1]);
			props.sampleRate = Audio::EndianConverterLong(fileProps[sampleRate], fileProps[sampleRate + 1],
				fileProps[sampleRate + 2], fileProps[sampleRate + 3]);
			props.byteRate = Audio::EndianConverterLong(fileProps[byteRate], fileProps[byteRate + 1],
				fileProps[byteRate + 2], fileProps[byteRate + 3]);
			props.blockAlign = Audio::EndianConverterShort(fileProps[blockAlign], fileProps[blockAlign + 1]);
			props.bitDepth = Audio::EndianConverterShort(fileProps[bitDepth], fileProps[bitDepth + 1]);
		}

		unsigned int dataPosition = 0;
		const int dataChunkPos = 4;
		char dataChunk[Audio::WAVDataSize] = {};
		dataChunk[dataChunkPos] = '\0';
		for (unsigned int j = fmtPosition; j < size; j++)
		{
			file.seekg(j);
			file.get(data);

			if (data == 'd' && j + Audio::WAVFormatSize < size) 
			{
				file.seekg(j);
				file.read(dataChunk, 4);
				if (std::strcmp("data", dataChunk) == 0) 
				{
					file.seekg(j);
					file.read(dataChunk, Audio::WAVDataSize);
					unsigned int dataSize = 0;
					char dataTemp = dataChunk[dataChunkPos];
					dataChunk[dataChunkPos] = dataTemp;
					if (Audio::IsBigEndian())
					{
						dataSize = *reinterpret_cast<unsigned int*>(dataChunk);
					}
					else 
					{
						dataSize = Audio::EndianConverterLong(dataChunk[dataChunkPos], dataChunk[dataChunkPos + 1],
							dataChunk[dataChunkPos + 2], dataChunk[dataChunkPos + 3]);
					}

					// Some applications I've found do not properly set the size parameters in their WAV headers
					// So this is a fail safe
					if (dataSize == (unsigned int)0xFFFFFFFF)
						dataSize = (unsigned int)std::filesystem::file_size(filePath);

					std::string fileName = std::filesystem::path(filePath).stem().string();
					info.filePath = filePath;
					info.name = fileName;
					info.props = props;
					info.dataSize = dataSize;
					info.dataZero = (unsigned int)file.tellg();
					return info;
				}
			}
		}
		file.close();
		return info;
	}

}