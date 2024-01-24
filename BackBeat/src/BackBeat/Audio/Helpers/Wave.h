#pragma once

namespace BackBeat {

	enum class WaveType
	{
		None = 0,
		Sin, 
		SawtoothUp, 
		SawtoothDown, 
		Triangle, 
		Square
	};

	class Wave
	{
	public:
		static void GetSinWave(float* buffer, unsigned int bufferSize, unsigned int numChannels);
		static void GetSawtoothUpWave(float* buffer, unsigned int bufferSize, unsigned int numChannels);
		static void GetSawtoothDownWave(float* buffer, unsigned int bufferSize, unsigned int numChannels);
		static void GetTriangleWave(float* buffer, unsigned int bufferSize, unsigned int numChannels);
		static void GetSquareWave(float* buffer, unsigned int bufferSize, unsigned int numChannels);
	private:
		static float ConvertRadians(float degrees);
	};
}