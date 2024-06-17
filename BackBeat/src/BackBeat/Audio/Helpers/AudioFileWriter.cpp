#include "bbpch.h"

#include "AudioFileWriter.h"
namespace BackBeat {

	bool AudioFileWriter::WriteWAVFileHeader(std::string filePath, AudioProps props, unsigned long dataSize)
	{
		std::ofstream file;
		file.open(filePath, std::ios::binary);
		if (file.is_open())
		{
			const bool bigEndian = Audio::IsBigEndian();
			// Write the header chunk
			char header[Audio::WAVHeaderSize] = {};
			const int headerSizeOffset = 8;
			unsigned long size = Audio::WAVTotalHeaderSize + dataSize - headerSizeOffset;
			auto sizePtr = reinterpret_cast<char*>(&size);
			
			header[0] = 'R';
			header[1] = 'I';
			header[2] = 'F';
			if (props.bigEndian)
				header[3] = 'X';
			else
				header[3] = 'F';
			
			if (bigEndian)
			{
				header[4] = sizePtr[3];
				header[5] = sizePtr[2];
				header[6] = sizePtr[1];
				header[7] = sizePtr[0];
			}
			else
			{
				header[4] = sizePtr[0];
				header[5] = sizePtr[1];
				header[6] = sizePtr[2];
				header[7] = sizePtr[3];
			}

			header[8] = 'W';
			header[9] = 'A';
			header[10] = 'V';
			header[11] = 'E';

			file.write(header, Audio::WAVHeaderSize);

			// Write the format subchunk
			char format[Audio::WAVFormatSize] = {};
			const int formatSize = 4;
			const int audioFormat = 8;
			const int numChannels = 10;
			const int sampleRate = 12;
			const int byteRate = 16;
			const int blockAlign = 20;
			const int bitDepth = 22;		
			// NOTE: In most cases, 16 is the size but some WAV files are different and specify the size differently to add more information
			unsigned long fmtSize = 16;
			auto fmtSizePtr = reinterpret_cast<char*>(&fmtSize);
			unsigned short propsFormat = props.format;
			auto formatPtr = reinterpret_cast<char*>(&propsFormat);
			unsigned short nChannels = props.numChannels;
			auto nChannelPtr = reinterpret_cast<char*>(&nChannels);
			unsigned long sRate = props.sampleRate;
			auto sRatePtr = reinterpret_cast<char*>(&sRate);
			unsigned long bRate = props.byteRate;
			auto bRatePtr = reinterpret_cast<char*>(&bRate);
			unsigned short blckAlign = props.blockAlign;
			auto blckAlignPtr = reinterpret_cast<char*>(&blckAlign);
			unsigned short bDepth = props.bitDepth;
			auto bDepthPtr = reinterpret_cast<char*>(&bDepth);

			format[0] = 'f';
			format[1] = 'm';
			format[2] = 't';
			format[3] = ' ';

			if (Audio::IsBigEndian())
			{
				format[formatSize] = fmtSizePtr[3];
				format[formatSize + 1] = fmtSizePtr[2];
				format[formatSize + 2] = fmtSizePtr[1];
				format[formatSize + 3] = fmtSizePtr[0];

				format[audioFormat] = formatPtr[1];
				format[audioFormat + 1] = formatPtr[0];

				format[numChannels] = nChannelPtr[1];
				format[numChannels + 1] = nChannelPtr[0];

				format[sampleRate] = sRatePtr[3];
				format[sampleRate + 1] = sRatePtr[2];
				format[sampleRate + 2] = sRatePtr[1];
				format[sampleRate + 3] = sRatePtr[0];

				format[byteRate] = bRatePtr[3];
				format[byteRate + 1] = bRatePtr[2];
				format[byteRate + 2] = bRatePtr[1];
				format[byteRate + 3] = bRatePtr[0];

				format[blockAlign] = blckAlignPtr[1];
				format[blockAlign + 1] = blckAlignPtr[0];

				format[bitDepth] = bDepthPtr[1];
				format[bitDepth + 1] = bDepthPtr[0];
			}
			else
			{
				format[formatSize] = fmtSizePtr[0];
				format[formatSize + 1] = fmtSizePtr[1];
				format[formatSize + 2] = fmtSizePtr[2];
				format[formatSize + 3] = fmtSizePtr[3];

				format[audioFormat] = formatPtr[0];
				format[audioFormat + 1] = formatPtr[1];

				format[numChannels] = nChannelPtr[0];
				format[numChannels + 1] = nChannelPtr[1];

				format[sampleRate] = sRatePtr[0];
				format[sampleRate + 1] = sRatePtr[1];
				format[sampleRate + 2] = sRatePtr[2];
				format[sampleRate + 3] = sRatePtr[3];

				format[byteRate] = bRatePtr[0];
				format[byteRate + 1] = bRatePtr[1];
				format[byteRate + 2] = bRatePtr[2];
				format[byteRate + 3] = bRatePtr[3];

				format[blockAlign] = blckAlignPtr[0];
				format[blockAlign + 1] = blckAlignPtr[1];

				format[bitDepth] = bDepthPtr[0];
				format[bitDepth + 1] = bDepthPtr[1];
			}

			file.write(format, Audio::WAVFormatSize);

			// Write the data subchunk
			char dataSubchunk[Audio::WAVDataSize] = {};
			const int dataSizePos = 4;
			unsigned long dSize = dataSize;
			auto dSizePtr = reinterpret_cast<char*>(&dSize);
			dataSubchunk[0] = 'd';
			dataSubchunk[1] = 'a';
			dataSubchunk[2] = 't';
			dataSubchunk[3] = 'a';

			if (bigEndian)
			{
				dataSubchunk[dataSizePos] = dSizePtr[3];
				dataSubchunk[dataSizePos + 1] = dSizePtr[2];
				dataSubchunk[dataSizePos + 2] = dSizePtr[1];
				dataSubchunk[dataSizePos + 3] = dSizePtr[0];
			}
			else
			{
				dataSubchunk[dataSizePos] = dSizePtr[0];
				dataSubchunk[dataSizePos + 1] = dSizePtr[1];
				dataSubchunk[dataSizePos + 2] = dSizePtr[2];
				dataSubchunk[dataSizePos + 3] = dSizePtr[3];
			}
			file.write(dataSubchunk, Audio::WAVDataSize);

			file.close();
			return true;
		}
		else
		{
			return false;
		}
	}

	// Sample (.smpl) files are written in little endian
	bool AudioFileWriter::WriteSampleFileHeader(std::string filePath, AudioProps props, unsigned int dataSize)
	{
		std::ofstream file;
		file.open(filePath, std::ios::binary);
		if (file.is_open())
		{
			const bool bigEndian = Audio::IsBigEndian();

			// Write the header chunk
			const int headerSizeOffset = 6;
			char header[Audio::SAMPLEHeaderSize] = {};
			unsigned long size = Audio::SAMPLETotalHeaderSize + dataSize - headerSizeOffset;
			auto sizePtr = reinterpret_cast<char*>(&size);
			
			header[0] = 'S';
			header[1] = 'A';
			header[2] = 'M';
			header[3] = 'P';
			header[4] = 'L';
			header[5] = 'E';

			if (bigEndian)
			{
				header[6] = sizePtr[3];
				header[7] = sizePtr[2];
				header[8] = sizePtr[1];
				header[9] = sizePtr[0];
			}
			else
			{
				header[6] = sizePtr[0];
				header[7] = sizePtr[1];
				header[8] = sizePtr[2];
				header[9] = sizePtr[3];
			}
			file.write(header, Audio::SAMPLEHeaderSize);

			// Write the format subchunk (formatted the same as WAV files)
			char format[Audio::SAMPLEFormatSize] = {};
			const int formatSize = 4;
			const int audioFormat = 8;
			const int numChannels = 10;
			const int sampleRate = 12;
			const int byteRate = 16;
			const int blockAlign = 20;
			const int bitDepth = 22;
			unsigned long fmtSize = 16;
			auto fmtSizePtr = reinterpret_cast<char*>(&fmtSize);
			unsigned short propsFormat = props.format;
			auto formatPtr = reinterpret_cast<char*>(&propsFormat);
			unsigned short nChannels = props.numChannels;
			auto nChannelPtr = reinterpret_cast<char*>(&nChannels);
			unsigned long sRate = props.sampleRate;
			auto sRatePtr = reinterpret_cast<char*>(&sRate);
			unsigned long bRate = props.byteRate;
			auto bRatePtr = reinterpret_cast<char*>(&bRate);
			unsigned short blckAlign = props.blockAlign;
			auto blckAlignPtr = reinterpret_cast<char*>(&blckAlign);
			unsigned short bDepth = props.bitDepth;
			auto bDepthPtr = reinterpret_cast<char*>(&bDepth);

			format[0] = 'f';
			format[1] = 'm';
			format[2] = 't';
			format[3] = ' ';

			if (Audio::IsBigEndian())
			{
				format[formatSize] = fmtSizePtr[3];
				format[formatSize + 1] = fmtSizePtr[2];
				format[formatSize + 2] = fmtSizePtr[1];
				format[formatSize + 3] = fmtSizePtr[0];

				format[audioFormat] = formatPtr[1];
				format[audioFormat + 1] = formatPtr[0];

				format[numChannels] = nChannelPtr[1];
				format[numChannels + 1] = nChannelPtr[0];

				format[sampleRate] = sRatePtr[3];
				format[sampleRate + 1] = sRatePtr[2];
				format[sampleRate + 2] = sRatePtr[1];
				format[sampleRate + 3] = sRatePtr[0];

				format[byteRate] = bRatePtr[3];
				format[byteRate + 1] = bRatePtr[2];
				format[byteRate + 2] = bRatePtr[1];
				format[byteRate + 3] = bRatePtr[0];

				format[blockAlign] = blckAlignPtr[1];
				format[blockAlign + 1] = blckAlignPtr[0];

				format[bitDepth] = bDepthPtr[1];
				format[bitDepth + 1] = bDepthPtr[0];
			}
			else
			{
				format[formatSize] = fmtSizePtr[0];
				format[formatSize + 1] = fmtSizePtr[1];
				format[formatSize + 2] = fmtSizePtr[2];
				format[formatSize + 3] = fmtSizePtr[3];

				format[audioFormat] = formatPtr[0];
				format[audioFormat + 1] = formatPtr[1];

				format[numChannels] = nChannelPtr[0];
				format[numChannels + 1] = nChannelPtr[1];

				format[sampleRate] = sRatePtr[0];
				format[sampleRate + 1] = sRatePtr[1];
				format[sampleRate + 2] = sRatePtr[2];
				format[sampleRate + 3] = sRatePtr[3];

				format[byteRate] = bRatePtr[0];
				format[byteRate + 1] = bRatePtr[1];
				format[byteRate + 2] = bRatePtr[2];
				format[byteRate + 3] = bRatePtr[3];

				format[blockAlign] = blckAlignPtr[0];
				format[blockAlign + 1] = blckAlignPtr[1];

				format[bitDepth] = bDepthPtr[0];
				format[bitDepth + 1] = bDepthPtr[1];
			}

			file.write(format, Audio::WAVFormatSize);

			// Write the data subchunk (formatted the same as WAV files)
			char dataSubchunk[Audio::SAMPLEDataSize] = {};
			const int dataSizePos = 4;
			unsigned long dSize = dataSize;
			auto dSizePtr = reinterpret_cast<char*>(&dSize);
			dataSubchunk[0] = 'd';
			dataSubchunk[1] = 'a';
			dataSubchunk[2] = 't';
			dataSubchunk[3] = 'a';

			if (bigEndian)
			{
				dataSubchunk[dataSizePos] = dSizePtr[3];
				dataSubchunk[dataSizePos + 1] = dSizePtr[2];
				dataSubchunk[dataSizePos + 2] = dSizePtr[1];
				dataSubchunk[dataSizePos + 3] = dSizePtr[0];
			}
			else
			{
				dataSubchunk[dataSizePos] = dSizePtr[0];
				dataSubchunk[dataSizePos + 1] = dSizePtr[1];
				dataSubchunk[dataSizePos + 2] = dSizePtr[2];
				dataSubchunk[dataSizePos + 3] = dSizePtr[3];
			}
			file.write(dataSubchunk, Audio::SAMPLEDataSize);

			file.close();
			return true;
		}
		else
			return false;
	}

	bool AudioFileWriter::WriteAudioFileData(std::string filePath, char* data, unsigned int numBytes)
	{
		std::ofstream file;
		file.open(filePath, std::ios::binary | std::ios::app);
		if (file.is_open())
		{
			file.write(data, numBytes);
			file.close();
			return true;
		}
		return false;
	}
}