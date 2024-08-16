#pragma once

// NOTES: - Only available using floats as that is the only data type used for calculations inside BackBeat
//        - Not a traditional Ring/Circular buffer as BackBeat needs non-uniform read and write sizes;
namespace BackBeat {

	// BackBeat runs in 48000 sampler rate stereo
	// 1ms = 48 samples
	// Low latency = ~5-10ms
	// 

	// This is about 1 second worth of audio data using BackBeat default properties of 48k sampler rate, stereo, and float bit depth
	constexpr unsigned int ActualSize = 200000;
	constexpr unsigned int MaxBufferSize = 10000;

	class CircularBuffer
	{
	public:
		CircularBuffer();
		~CircularBuffer();

		bool Write(float* data, unsigned int num);
		bool Read(float* data, unsigned int num);
		void Reset();

	private:
		unsigned int m_HeadsIndex;
		unsigned int m_TailsIndex;
		float m_Array[ActualSize] = {};

	};


}