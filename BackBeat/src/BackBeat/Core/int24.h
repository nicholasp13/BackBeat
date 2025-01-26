#pragma once

// Signed, two's complement 24 bit integer, always big endian

namespace BackBeat {

	typedef unsigned char byte;

	// Taken from IBM developer
	static bool IsBigEndian()
	{
		int i = 1;
		char* p = (char*)&i;
		return !p[0] == 1;
	}

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
		static const unsigned int s_ByteBitSize = 8,
			s_Int24ByteSize = 3;

		byte m_SignedByte;
		byte m_UpperByte;
		byte m_LowerByte;

	public:
		static int24* GetInt24Buffer(byte* buffer, unsigned int numInts, bool bigEndian);
		static byte* GetByteBuffer(int24* buffer, unsigned int numInts, bool bigEndian);

	};

}