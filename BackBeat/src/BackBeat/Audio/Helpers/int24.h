#pragma once

// Signed, two's complement 24 bit integer, always big endian

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

		int24 operator ++ () { return *this + int24(1); }
		int24 operator -- () { return *this - int24(1); }
		int24 operator += (int24 num) { return *this + num; }
		int24 operator -= (int24 num) { return *this - num; }
		int24 operator *= (int24 num) { return *this * num; }
		int24 operator /= (int24 num) { return *this / num; }
		bool operator != (int24 num) { return !(*this == num); }
		bool operator <= (int24 num) { return (*this < num) || (*this == num); }
		bool operator > (int24 num) { return !(*this <= num); }
		bool operator >= (int24 num) { return !(*this < num); }

		short ToShort() { return (short)ToLong(); }
		int ToInt() { return (int)ToLong(); }
		float ToFloat() { return (float)ToLong(); }
		double ToDouble() { return (double)ToLong(); }

	private:
		byte m_SignedByte;
		byte m_UpperByte;
		byte m_LowerByte;

	};

}