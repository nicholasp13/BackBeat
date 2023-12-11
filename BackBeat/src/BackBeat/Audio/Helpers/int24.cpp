#include "bbpch.h"

#include "int24.h"
namespace BackBeat {

	int24::int24()
		: m_SignedByte(0x00), m_UpperByte(0x00), m_LowerByte(0)
	{

	}
	
	int24::int24(byte signedByte, byte upperByte, byte lowerByte)
		: m_SignedByte(signedByte), m_UpperByte(upperByte), m_LowerByte(lowerByte)
	{

	}
	
	int24::int24(short num)
	{
		auto x = reinterpret_cast<byte*>(&num);
		if (Audio::IsBigEndian()) {
			m_UpperByte = x[0];
			m_LowerByte = x[1];
		}
		else {
			m_UpperByte = x[1];
			m_LowerByte = x[0];
		}
		
		if (num >= 0) {
			m_SignedByte = 0x00;
		}
		else {
			m_SignedByte = 0xFF;
		}
	}

	int24::int24(long num)
	{
		auto x = reinterpret_cast<byte*>(&num);

		if (Audio::IsBigEndian()) {
			m_SignedByte = x[0];
			m_UpperByte = x[1];
			m_LowerByte = x[2];
		}
		else {
			m_SignedByte = x[2];
			m_UpperByte = x[1];
			m_LowerByte = x[0];
		}
	}

	int24::int24(int num)
	{
		auto x = reinterpret_cast<byte*>(&num);

		if (Audio::IsBigEndian()) {
			m_SignedByte = x[0];
			m_UpperByte = x[1];
			m_LowerByte = x[2];
		}
		else {
			m_SignedByte = x[2];
			m_UpperByte = x[1];
			m_LowerByte = x[0];
		}
	}

	int24::int24(float num)
	{
		auto x = (long)num;
		auto y = reinterpret_cast<byte*>(&x);
		if (Audio::IsBigEndian()) {
			m_SignedByte = y[0];
			m_UpperByte = y[1];
			m_LowerByte = y[2];
		}
		else {
			m_SignedByte = y[2];
			m_UpperByte = y[1];
			m_LowerByte = y[0];
		}
	}

	int24::int24(double num)
	{
		auto x = (long)num;
		auto y = reinterpret_cast<byte*>(&x);
		if (Audio::IsBigEndian()) {
			m_SignedByte = y[0];
			m_UpperByte = y[1];
			m_LowerByte = y[2];
		}
		else {
			m_SignedByte = y[2];
			m_UpperByte = y[1];
			m_LowerByte = y[0];
		}
	}

	int24::~int24()
	{

	}

	int24 int24::operator + (int24 num)
	{
		long x = ToLong();
		long y = num.ToLong();
		return int24(x + y);
	}
	
	int24 int24::operator - (int24 num)
	{
		long x = ToLong();
		long y = num.ToLong();
		return int24(x - y);
	}

	int24 int24::operator * (int24 num)
	{
		long x = ToLong();
		long y = num.ToLong();
		return int24(x * y);
	}
	
	int24 int24::operator / (int24 num)
	{
		long x = ToLong();
		long y = num.ToLong();
		return int24(x / y);
	}
	
	
	bool int24::operator == (int24 num)
	{
		return (m_SignedByte == num[0]) && (m_UpperByte == num[1]) && (m_LowerByte == num[2]);
	}
	
	bool int24::operator < (int24 num)
	{
		long x = ToLong();
		long y = num.ToLong();
		return x < y;
	}

	byte int24::operator [] (int num)
	{
		if (num == 0)
			return m_SignedByte;
		else if (num == 1)
			return m_UpperByte;
		else if (num == 2)
			return m_LowerByte;
		else
			throw std::out_of_range("int24 ERROR: Out of range");
	}
	

	signed long int24::ToLong()
	{
		signed long negativeByte = 0x00;
		signed long signedByte = 0x00;
		signed long upperByte = 0x00;
		signed long lowerByte = 0x00;
		
		if (*this == int24())
			return long(0);

		if (Audio::IsBigEndian()) {
			negativeByte = 0xFF;
			signedByte = m_SignedByte << BYTE_BIT_SIZE;
			upperByte = m_UpperByte << BYTE_BIT_SIZE * 2;
			lowerByte = m_LowerByte << BYTE_BIT_SIZE * 3;
			if (IsPositive()) {
				return signedByte | upperByte | lowerByte;
			}
			else {
				return negativeByte | signedByte | upperByte | lowerByte;
			}
		}
		else {
			negativeByte = 0xFF << BYTE_BIT_SIZE * 3;
			signedByte = m_SignedByte << BYTE_BIT_SIZE * 2;
			upperByte = m_UpperByte << BYTE_BIT_SIZE;
			lowerByte = m_LowerByte;

			if (IsPositive()) {
				return signedByte | upperByte | lowerByte;
			}
			else {
				return negativeByte | signedByte | upperByte | lowerByte;
			}
		}
	}

	bool int24::IsPositive()
	{
		if (*this != int24()) {
			byte signedBit = 0x80;
			return (m_SignedByte & signedBit) == 0x00;
		}
		return false;
	}

}