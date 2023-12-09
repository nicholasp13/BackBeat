#pragma once

// Signed, two's complement 24 bit integer, always big endian

#include "BackBeat/Audio/Audio.h"
namespace BackBeat {

	class int24 
	{
	public:
		int24(); // Checked
		int24(byte signedByte, byte upperByte, byte lowerByte); // Checked
		int24(short num); // Checked
		int24(long num); // Checked
		int24(float num);
		int24(double num);
		~int24();

		int24 operator + (int24 num);
		int24 operator - (int24 num);
		int24 operator * (int24 num);
		int24 operator / (int24 num);
		bool operator == (int24 num); // Checked
		byte operator [] (int num);   // Checked

		float toFloat();
		signed long toLong(); // Checked

		bool IsPositive(); 

		bool operator != (int24 num) { return !(*this == num); }

	private:
		byte m_SignedByte;
		byte m_UpperByte;
		byte m_LowerByte;

	};

}