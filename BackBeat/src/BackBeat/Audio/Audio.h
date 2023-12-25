#pragma once

#include "BackBeat/Core/Core.h"
namespace BackBeat {

// MISC CONSTANTS
#define BYTE_BIT_SIZE     8
#define INT16_BIT_SIZE    16
#define INT24_BIT_SIZE    24
#define FLOAT_BIT_SIZE    32
#define DOUBLE_BIT_SIZE   64
#define INT24_BYTE_SIZE   3
#define INT24_MAX         8388607.0f

// AUDIOFILE CONSTANTS
#define MONO    (UINT32)1
#define STEREO  (UINT32)2
#define SHORT_INT_MAX   32676
#define WAV_HEADER_SIZE 12
#define WAV_FMT_SIZE    24
#define WAV_DATA_SIZE   8

// AUDIO FILE STRINGS
#define MP3 "ID3"
#define WAV "RIF"

// REFERENCE_TIME time units per second and per millisecond
#define REFTIMES_PER_SECOND   10000000
#define REFTIMES_PER_MILLISEC 10000

// BackBeat HRESULT FAILURE CODES
#define PLAY_FAILURE (HRESULT)1001
#define LOAD_FAILURE (HRESULT)1002
#define MIX_FAILURE  (HRESULT)1003

// NOTE: ONLY USED FOR WINDOWS API
// TODO: Expand/Make another CHECK_FAILURE for MMRESULT (Midi Input device Windows error messages) 
#define CHECK_FAILURE( hr ) \
	if (hr == PLAY_FAILURE) \
	{ BB_CORE_ERROR("{0} FAILED TO PLAY", hr);  return; } \
	else if (hr == LOAD_FAILURE) \
	{ BB_CORE_ERROR("{0} FAILED TO LOAD", hr); return; } \
	else if (hr == MIX_FAILURE) \
	{ BB_CORE_ERROR("{0} FAILED TO MIX", hr); return;} \
	else if (FAILED(hr) != S_OK) \
	{ BB_CORE_ERROR("{0} WINDOWS WASAPI API FAILURE", hr); return; }

	typedef unsigned char byte;

	enum FileType {
		none = 0,
		wav,
		mp3
	};

	struct AudioProps {
		bool bigEndian;
		unsigned short format;
		unsigned short numChannels;
		unsigned long sampleRate;
		unsigned long byteRate;
		unsigned short blockAlign;  // numChannels * bitDepth / 8
		unsigned short bitDepth;
		unsigned long fileSize;
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

	struct TimeMinSec {
		unsigned int minutes;
		unsigned int seconds;
	};

	// NOTE: May want to move this to a Helpers.h file in the Helpers folder for better organization, not much sense to have these functions here
	//       other than consolidation
	class Audio
	{
	public:
		static unsigned short EndianConverterShort(char num1, char num2)
		{
			unsigned short leftToRight = (num1 & 0x00FF);
			unsigned short rightToLeft = (num2 & 0x00FF) << BYTE_BIT_SIZE;
			return (leftToRight | rightToLeft);
		}

		static unsigned long EndianConverterLong(char num1, char num2,
			char num3, char num4)
		{
			unsigned long leftMost = (num1 & 0x000000FF);
			unsigned long left = (num2 & 0x000000FF) << BYTE_BIT_SIZE;
			unsigned long right = (num3 & 0x000000FF) << (2 * BYTE_BIT_SIZE);
			unsigned long rightMost = (num4 & 0x000000FF) << (3 * BYTE_BIT_SIZE);
			return (left | leftMost | right | rightMost);
		}

		static short EndianConverterSignedShort(char num1, char num2)
		{
			short leftToRight = (num1 & 0x00FF);
			short rightToLeft = (num2 & 0x00FF) << BYTE_BIT_SIZE;
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

		// Note: Not currently used as most buffers need either to recast/type convert or are shared pointers
		template<typename T>
		static void CopyInputToOutput(T inputBuffer, T outputBuffer, UINT32 bytesToCopy)
		{
			memcpy(reinterpret_cast<void*>(inputBuffer), reinterpret_cast<void*>(outputBuffer), bytesToCopy);
		}

		static void FlushBuffer(std::shared_ptr<float[]> buffer, UINT32 numSamples, UINT32 numChannels, float defaultValue)
		{
			for (UINT32 i = 0; i < numSamples * numChannels; i++)
				buffer[i] = defaultValue;
		}

		// Taken from IBM developer
		static bool IsBigEndian()
		{
			int i = 1;
			char *p = (char*) & i;

			if (p[0] == 1)
				return false;
			else
				return true;
		}

		static TimeMinSec GetTime(float totalSeconds)
		{
			TimeMinSec time = TimeMinSec();
			unsigned int minutes = (unsigned int)floor(totalSeconds / 60.0f);
			unsigned int seconds = (unsigned int)((unsigned int)totalSeconds % 60);
			time.minutes = minutes;
			time.seconds = seconds;
			return time;
		}
	};
}