#pragma once

#include "BackBeat/Core/Core.h"

#include <mmdeviceapi.h>
#include <Audioclient.h>
#include <AudioSessionTypes.h>
#include <synchapi.h>

// TODO add helper functions/ failure checks and HRESULT functions
namespace BackBeat {

// 
#define BYTESIZE 8
#define SHORT_INT_MAX 32676
#define WAV_HEADER_SIZE 44
#define MP3 "ID3"
#define WAV "RIFF"

// REFERENCE_TIME time units per second and per millisecond
#define REFTIMES_PER_SECOND 10000000
#define REFTIMES_PER_MILLISEC 10000

// HRESULT FAILURE CODES
#define PLAY_FAILURE (HRESULT)1
#define LOAD_FAILURE (HRESULT)2
#define MIX_FAILURE  (HRESULT)3

#define CHECK_FAILURE( hr ) \
	if (hr == PLAY_FAILURE) \
	{ BB_CORE_ERROR("{0} FAILED TO PLAY", hr); return; } \
	else if (hr == LOAD_FAILURE) \
	{ BB_CORE_ERROR("{0} FAILED TO LOAD", hr); return; } \
	else if (hr == MIX_FAILURE) \
	{ BB_CORE_ERROR("{0} FAILED TO MIX", hr); return;} \
	else if (FAILED(hr)) \
	{ BB_CORE_ERROR("{0} WINDOWS WASAPI API FAILURE", hr); return; }

#define FILE_OPENED( fileOpened ) \
	if (!fileOpened) \
	{ BB_CORE_ERROR("FAILED TO OPEN FILE"); FileOpened = false; Playing = false; return; }

	class Audio
	{
	public:
		static unsigned short EndianConverterShort(char num1, char num2)
		{
			unsigned short leftToRight = (num1 & 0x00FF);
			unsigned short rightToLeft = (num2 & 0x00FF) << BYTESIZE;
			return (leftToRight | rightToLeft);
		}

		static unsigned long EndianConverterLong(char num1, char num2,
			char num3, char num4)
		{
			unsigned long leftMost = (num1 & 0x000000FF);
			unsigned long left = (num2 & 0x000000FF) << BYTESIZE;
			unsigned long right = (num3 & 0x000000FF) << (2 * BYTESIZE);
			unsigned long rightMost = (num4 & 0x000000FF) << (3 * BYTESIZE);
			return (left | leftMost | right | rightMost);
		}

		static short EndianConverterSignedShort(char num1, char num2)
		{
			short leftToRight = (num1 & 0x00FF);
			short rightToLeft = (num2 & 0x00FF) << BYTESIZE;
			return (leftToRight | rightToLeft);
		}

		template<typename T>
		static float ConvertFloat(T value) {
			return (float)(value);
		}
	};
}