#pragma once

// Signed, two's complement 24 bit integer, always big endian

// TODO: Reverse dependency on Audio as int24 should be included in Audio.h not the other way around

#include "BackBeat/Audio/Audio.h"
namespace BackBeat {

	class int24 
	{
	public:
		int24();
		int24(byte signedByte, byte upperByte, byte lowerByte);
		int24(short num);
		int24(long num);
		int24(int num);
		int24(float num);
		int24(double num);
		~int24();

		int24 operator + (int24 num);
		int24 operator - (int24 num);
		int24 operator * (int24 num);
		int24 operator / (int24 num);
		bool operator == (int24 num);
		bool operator < (int24 num);
		byte operator [] (int num);

		signed long ToLong();
		bool IsPositive();

		inline int24 operator ++ () { return *this + int24(1); }
		inline int24 operator -- () { return *this - int24(1); }
		inline int24 operator += (int24 num) { return *this + num; }
		inline int24 operator -= (int24 num) { return *this - num; }
		inline int24 operator *= (int24 num) { return *this * num; }
		inline int24 operator /= (int24 num) { return *this / num; }
		inline bool operator != (int24 num) { return !(*this == num); }
		inline bool operator <= (int24 num) { return (*this < num) || (*this == num); }
		inline bool operator > (int24 num) { return !(*this <= num); }
		inline bool operator >= (int24 num) { return !(*this < num); }
		inline operator short() { return ToShort(); }
		inline operator long() { return ToLong(); }
		inline operator int() { return ToInt(); }
		inline operator float() { return ToFloat(); }
		inline operator double() { return ToDouble(); }

		inline short ToShort() { return (short)ToLong(); }
		inline int ToInt() { return (int)ToLong(); }
		inline float ToFloat() { return (float)ToLong(); }
		inline double ToDouble() { return (double)ToLong(); }

	private:
		byte m_SignedByte;
		byte m_UpperByte;
		byte m_LowerByte;

	public:
		static int24* GetInt24Buffer(byte* buffer, unsigned int numInts, bool bigEndian);
		static byte* GetByteBuffer(int24* buffer, unsigned int numInts, bool bigEndian);

		// NOTE(s): - Due to the way template functions are. This implementation, using include on a .cpp file,
		//       or creating seperate functions for each type are the only solutions
		//          - Allocating memory on the heap during important run time ESPECIALLY on the audio thread which requires
		//       exact timing is generally looked down on. However in this case, this code is baked in here and in 
		//       SampleBuilder and would take a bit of reworking to fix so this is for future reference in case I forget.
		//       (Another thing is that creating an array or pointer of the class as a private member caused nontrivial compiler 
		//       errors that made this tough to solve)
		template<typename T>
		static void TranslateDataToInt24(T inBuffer, byte* outputBuffer, unsigned int inBitDepth,
			unsigned int numChannels, unsigned int numSamples, bool bigEndian)
		{
			float depthRatio = Audio::GetTypeRatio(Audio::FloatBitSize, inBitDepth);

			int24* intBuffer = int24::GetInt24Buffer(outputBuffer, numChannels * numSamples, bigEndian);
			for (unsigned int i = 0; i < numSamples * numChannels; i += numChannels)
			{
				for (unsigned int j = 0; j < numChannels; j++)
				{
					intBuffer[i + j] += int24((float)(inBuffer[i + j]) * depthRatio);
				}
			}
			byte* byteBuffer = int24::GetByteBuffer(intBuffer, numChannels * numSamples, bigEndian);
			Audio::CopyInputToOutput(outputBuffer, byteBuffer, numChannels * numSamples * Audio::Int24ByteSize);
			delete[] intBuffer;
			delete[] byteBuffer;
		}

	};

}