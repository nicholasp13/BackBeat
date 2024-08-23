#pragma once

#include <stdatomic.h>

// NOTES: - Only available using floats as that is the only data type used for calculations inside BackBeat
//        - Not a traditional Ring/Circular buffer as BackBeat needs non-uniform read and write sizes;
namespace BackBeat {

	class CircularBuffer
	{
	public:
		CircularBuffer();
		~CircularBuffer();

		bool Write(float* data, unsigned int num);
		bool Read(float* data, unsigned int num);
		void Reset();

	private:
		static const unsigned int s_ArraySize = 48000;
		static const unsigned int s_MaxBufferSize = 200000;

		std::atomic<unsigned int> m_HeadsIndex;
		std::atomic<unsigned int> m_TailsIndex;

		float m_Array[s_ArraySize] = {};

	};


}