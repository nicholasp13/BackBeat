#include "bbpch.h"

#include "BackBeat/Core/Log.h"
#include "CircularBuffer.h"
namespace BackBeat {

	CircularBuffer::CircularBuffer()
		:
		m_HeadsIndex(0),
		m_TailsIndex(0)
	{
		
	}

	CircularBuffer::~CircularBuffer()
	{

	}

	bool CircularBuffer::Write(float* data, unsigned int num)
	{
		if (num > MaxBufferSize)
			return false;

		unsigned int newPos = (m_HeadsIndex + num) % ActualSize;
		if (m_HeadsIndex < m_TailsIndex && newPos >= m_TailsIndex)
			return false;
		else if (m_HeadsIndex > newPos && newPos >= m_TailsIndex)
			return false;

		if (newPos >= m_HeadsIndex)
		{
			unsigned int num1 = num * sizeof(float);
			memcpy(m_Array + m_HeadsIndex, data, std::size_t(num1));
			m_HeadsIndex = newPos;
		}
		else
		{
			unsigned int index = ActualSize - m_HeadsIndex;
			unsigned int num1 = index * sizeof(float);
			unsigned int num2 = newPos * sizeof(float);
			memcpy(m_Array + m_HeadsIndex, data, std::size_t(num1));
			if (newPos > 0)
				memcpy(m_Array, data + index, std::size_t(num2));
			m_HeadsIndex = newPos;
		}
		return true;
	}

	bool CircularBuffer::Read(float* data, unsigned int num)
	{
		if (num > MaxBufferSize)
			return false;

		unsigned int newPos = (m_TailsIndex + num) % ActualSize;
		if (m_TailsIndex <= m_HeadsIndex && newPos >= m_HeadsIndex)
			return false;
		else if (m_TailsIndex > newPos && newPos > m_HeadsIndex)
			return false;

		if (newPos >= m_TailsIndex)
		{
			unsigned int num1 = num * sizeof(float);
			memcpy(data, m_Array + m_TailsIndex, std::size_t(num1));
			m_TailsIndex = newPos;
		}
		else
		{
			unsigned int index = ActualSize - m_TailsIndex;
			unsigned int num1 = index * sizeof(float);
			unsigned int num2 = newPos * sizeof(float);
			memcpy(data, m_Array + m_TailsIndex, std::size_t(num1));
			if (newPos > 0)
				memcpy(data + index, m_Array, std::size_t(num2));
			m_TailsIndex = newPos;
		}
		return true;
	}

	void CircularBuffer::Reset()
	{
		m_HeadsIndex = 0;
		m_TailsIndex = 0;
	}

}