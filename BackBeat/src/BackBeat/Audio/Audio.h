#pragma once

#include "BackBeat/Audio/MIDI/MIDICodes.h"
#include "BackBeat/Core/Core.h"
namespace BackBeat {

	typedef unsigned char byte;

	enum class FileType {
		none = 0,
		wav,
		mp3,
		sample,
		recordingTemp
	};

	struct AudioProps {
		bool bigEndian;
		unsigned short format;      // 1 is uncompressed, pcm with 2's complement bit representation / 3 is uncompressed, pcm with floating point bit representation / All other numbers indicate some kind of compression
		unsigned short numChannels;
		unsigned long sampleRate;
		unsigned long byteRate;
		unsigned short blockAlign;  // numChannels * bitDepth / 8
		unsigned short bitDepth;
		unsigned long fileSize;

		// fileSize is irrelevant in nearly every case (every case in BackBeat) and is not checked here
		bool operator == (AudioProps props)
		{
			return (bigEndian == props.bigEndian)
				&& (format == props.format)
				&& (numChannels == props.numChannels)
				&& (sampleRate == props.sampleRate)
				&& (byteRate == props.byteRate)
				&& (blockAlign == props.blockAlign)
				&& (bitDepth == props.bitDepth);
		}

		inline bool operator != (AudioProps props) { return !(*this == props); }
	};

	struct AudioInfo {
		FileType type;
		std::string name;
		std::string filePath;
		AudioProps props;
		unsigned int dataZero;    // Data subchunk position of audio files is not uniform across all files
		unsigned int dataSize;    // Size of data subchunk (file size is stored in AudioProps)
	};

	// Struct of basic MIDI events
	struct MIDIEvent {
		byte status;
		byte data1;
		byte data2;
	};

	// TODO: Add other members as needed for other MIDI data i.e. pitch modulation
	struct NoteEvent {
		bool noteOn;
		int channel;
		float note;
		byte midiNote;
		byte velocity;
	};

	struct TimeMinSec {
		unsigned int minutes;
		unsigned int seconds;
		unsigned int milliseconds;
	};

	// NOTE: May want to move this to a Helpers.h file in the Helpers folder for better organization, not much sense to have these functions here
	//       other than consolidation
	namespace Audio {

		// MISC CONSTANTS
		constexpr unsigned int ByteBitSize   = 8;
		constexpr unsigned int Int16BitSize  = 16;
		constexpr unsigned int Int24BitSize  = 24;
		constexpr unsigned int FloatBitSize  = 32;
		constexpr unsigned int DoubleBitSize = 64;
		constexpr unsigned int Int24ByteSize = 3;
		constexpr unsigned int FloatByteSize = 4;
		constexpr float Int24Max             = 8388607.0f;

		// AUDIOFILE CONSTANTS
		constexpr unsigned int Mono                  = 1;
		constexpr unsigned int Stereo                = 2;
		constexpr unsigned int FormatUncompressed    = 1;
		constexpr unsigned int FormatTwosCompliement = 2;
		constexpr unsigned int FormatFloatingPoint   = 3;
		constexpr auto MP3 = "ID3";
		// WAV files
		constexpr unsigned int WAVHeaderSize      = 12;
		constexpr unsigned int WAVFormatSize      = 24;
		constexpr unsigned int WAVDataSize        = 8;
		constexpr unsigned int WAVTotalHeaderSize = 44;
		constexpr auto WAV = "RIF";
		// SAMPLE files
		constexpr unsigned int SAMPLEHeaderSize      = 10;
		constexpr unsigned int SAMPLEFormatSize      = 24;
		constexpr unsigned int SAMPLEDataSize        = 8;
		constexpr unsigned int SAMPLETotalHeaderSize = 42;
		const auto SAMPLE = "SAMPLE";

		static unsigned short EndianConverterShort(char num1, char num2)
		{
			unsigned short leftToRight = (num1 & 0x00FF);
			unsigned short rightToLeft = (num2 & 0x00FF) << ByteBitSize;
			return (leftToRight | rightToLeft);
		}

		static unsigned long EndianConverterLong(char num1, char num2,
			char num3, char num4)
		{
			unsigned long leftMost = (num1 & 0x000000FF);
			unsigned long left = (num2 & 0x000000FF) << ByteBitSize;
			unsigned long right = (num3 & 0x000000FF) << (2 * ByteBitSize);
			unsigned long rightMost = (num4 & 0x000000FF) << (3 * ByteBitSize);
			return (left | leftMost | right | rightMost);
		}

		static short EndianConverterSignedShort(char num1, char num2)
		{
			short leftToRight = (num1 & 0x00FF);
			short rightToLeft = (num2 & 0x00FF) << ByteBitSize;
			return (leftToRight | rightToLeft);
		}

		static bool EqualsFloat(float x, float y, float delta)
		{
			float value = sqrtf(powf(x - y, 2.0f));
			return (value < delta);
		}

		template<typename T>
		static float ConvertFloat(T value) 
		{
			return (float)(value);
		}

		template<typename T>
		static void CopyInputToOutput(T inputBuffer, T outputBuffer, unsigned int bytesToCopy)
		{
			memcpy(reinterpret_cast<void*>(outputBuffer), reinterpret_cast<void*>(inputBuffer), bytesToCopy);
		}

		static void FlushBuffer(std::shared_ptr<float[]> buffer, unsigned int numSamples, unsigned int numChannels, float defaultValue)
		{
			for (unsigned int i = 0; i < numSamples * numChannels; i++)
				buffer[i] = defaultValue;
		}

		template<typename T>
		static void FlushBufferT(T buffer, T defaultValue, unsigned int num)
		{
			for (unsigned int i = 0; i < num; i++)
				buffer[i] = *defaultValue;
		}

		// Taken from IBM developer
		static bool IsBigEndian()
		{
			int i = 1;
			char *p = (char*) & i;
			return !p[0] == 1;
		}

		// Gets time in minutes and seconds only. TODO: Refactor to reflect specific time units returned
		static TimeMinSec GetTime(float totalSeconds)
		{
			TimeMinSec time = TimeMinSec();
			unsigned int minutes = (unsigned int)floor(totalSeconds / 60.0f);
			unsigned int seconds = (unsigned int)((unsigned int)totalSeconds % 60);
			time.minutes = minutes;
			time.seconds = seconds;
			time.milliseconds = 0;
			return time;
		}

		static TimeMinSec SamplesToMs(unsigned int numSamples, unsigned int sampleRate)
		{
			TimeMinSec time = TimeMinSec();
			unsigned int sampleRateMs = sampleRate / 1000;
			unsigned int ms = (unsigned int)((float)numSamples / float(sampleRateMs));
			time.minutes = 0;
			time.seconds = 0;
			time.milliseconds = ms;
			return time;
		}

		static unsigned int MsToSamples(unsigned int ms, unsigned int sampleRate)
		{
			return ms * sampleRate / 1000;
		}

		static bool IsNoteOn(MIDIEvent event)
		{
			return (event.status <= MIDI::ChannelOn16 && event.status >= MIDI::ChannelOn1);
		}

		// Return values:
		// 1 is uncompressed, pcm with 2's complement bit representation 
		// 3 is uncompressed, pcm with floating point bit representation 
		// All other numbers indicate some kind of compression (It is defaulted to 0 as that is 
		static unsigned short GetAudioFormat(unsigned short bitDepth, bool compressed)
		{
			// Compression is currently unhandled by BackBeat and therefore returns 0 without further checks (t
			if (compressed)
				return 0;
			else if (bitDepth == FloatBitSize)
				return 3;
			else
				return 1;
		}

		// NOTE: Uses macros defined in stdint.h for int 8 and int 16 types. This does not currently cause any issues. 
		static float GetTypeRatio(unsigned short bitDepth1, unsigned short bitDepth2)
		{
			float max1 = 1.0f;
			float max2 = 1.0f;
			switch (bitDepth1)
			{

			case (ByteBitSize):
			{
				max1 = INT8_MAX;
				break;
			}

			case (Int16BitSize):
			{
				max1 = INT16_MAX;
				break;
			}

			case (Int24BitSize):
			{
				max1 = Int24Max;
				break;
			}

			case (FloatBitSize):
			{
				max1 = 1.0f;
				break;
			}

			case (DoubleBitSize):
			{
				max1 = 1.0f;
				break;
			}

			default:
			{
				return 0.0f;
			}

			}

			switch (bitDepth2)
			{

			case (ByteBitSize):
			{
				max2 = INT8_MAX;
				break;
			}

			case (Int16BitSize):
			{
				max2 = INT16_MAX;
				break;
			}

			case (Int24BitSize):
			{
				max2 = Int24Max;
				break;
			}

			case (FloatBitSize):
			{
				max2 = 1.0f;
				break;
			}

			case (DoubleBitSize):
			{
				max2 = 1.0f;
				break;
			}

			default:
			{
				return 0.0f;
			}

			}

			return max1 / max2;
		}

		template<typename T>
		static void TranslateDataToByte(T inBuffer, byte* outBuffer, unsigned int inBitDepth,
			unsigned int numChannels, unsigned int numSamples)
		{
			float depthRatio = GetTypeRatio(FloatBitSize, inBitDepth);

			for (unsigned int i = 0; i < numSamples * numChannels; i += numChannels)
			{
				for (unsigned int j = 0; j < numChannels; j++)
				{
					outBuffer[i + j] += (byte)((float)(inBuffer[i + j]) * depthRatio);
				}
			}
		}

		template<typename T>
		static void TranslateDataToShort(T inBuffer, signed short* outBuffer, unsigned int inBitDepth,
			unsigned int numChannels, unsigned int numSamples)
		{
			float depthRatio = GetTypeRatio(FloatBitSize, inBitDepth);

			for (unsigned int i = 0; i < numSamples * numChannels; i += numChannels)
			{
				for (unsigned int j = 0; j < numChannels; j++)
				{
					outBuffer[i + j] += (signed short)((float)(inBuffer[i + j]) * depthRatio);
				}
			}
		}

		template<typename T>
		static void TranslateDataToFloat(T inBuffer, float* outBuffer, unsigned int inBitDepth,
			unsigned int numChannels, unsigned int numSamples)
		{
			float depthRatio = GetTypeRatio(FloatBitSize, inBitDepth);

			for (unsigned int i = 0; i < numSamples * numChannels; i += numChannels)
			{
				for (unsigned int j = 0; j < numChannels; j++)
				{
					outBuffer[i + j] += (float)(inBuffer[i + j]) * depthRatio;
				}
			}
		}

		template<typename T>
		static void TranslateDataToDouble(T inBuffer, double* outBuffer, unsigned int inBitDepth,
			unsigned int numChannels, unsigned int numSamples)
		{
			float depthRatio = GetTypeRatio(FloatBitSize, inBitDepth);

			for (unsigned int i = 0; i < numSamples * numChannels; i += numChannels)
			{
				for (unsigned int j = 0; j < numChannels; j++)
				{
					outBuffer[i + j] += (double)(inBuffer[i + j]) * (double)depthRatio;
				}
			}
		}

		// TODO: Inverse the dependency of Int24
		// Currently does nothing with Int24 buffer values
		static void MultiplyBufferByValue(byte* buffer, unsigned int numBytes, AudioProps props, float value)
		{
			unsigned int numSamples = numBytes / props.blockAlign * props.numChannels;

			switch (props.bitDepth)
			{

			case (ByteBitSize):
			{
				for (unsigned int i = 0; i < numSamples; i++) {
					buffer[i] = (byte)((float)(buffer[i]) * value);
				}
				break;
			}

			case (Int16BitSize):
			{
				auto shortbuffer = reinterpret_cast<signed short*>(buffer);
				for (unsigned int i = 0; i < numSamples; i++) {
					shortbuffer[i] = (signed short)((float)(shortbuffer[i]) * value);
				}
				break;
			}

			case (Int24BitSize):
			{
				/**
				int24* intBuffer = int24::GetInt24Buffer(buffer, numSamples, props.bigEndian);
				for (unsigned int i = 0; i < numSamples; i++) {
					intBuffer[i] = int24((float)intBuffer[i] * value);
				}

				byte* byteBuffer = int24::GetByteBuffer(intBuffer, numSamples, props.bigEndian);
				Audio::CopyInputToOutput(buffer, byteBuffer, numSamples * Audio::Int24ByteSize);
				delete[] intBuffer;
				delete[] byteBuffer;
				break;
				/**/
			}

			case (FloatBitSize):
			{
				auto floatBuffer = reinterpret_cast<float*>(buffer);
				for (unsigned int i = 0; i < numSamples; i++) {
					floatBuffer[i] = floatBuffer[i] * value;
				}
				break;
			}

			case (DoubleBitSize):
			{
				auto doubleBuffer = reinterpret_cast<double*>(buffer);
				for (unsigned int i = 0; i < numSamples; i++) {
					doubleBuffer[i] = (double)(doubleBuffer[i] * value);
				}
				break;
			}

			default:
			{
				return;
			}

			}
		}

	}
}