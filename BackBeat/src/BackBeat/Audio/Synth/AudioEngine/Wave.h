#pragma once

// TODO:
// 
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
		static void GetSinWave(float* buffer, UINT32 bufferSize, UINT32 numChannels);
		static void GetSawtoothUpWave(float* buffer, UINT32 bufferSize, UINT32 numChannels);
		static void GetSawtoothDownWave(float* buffer, UINT32 bufferSize, UINT32 numChannels);
		static void GetTriangleWave(float* buffer, UINT32 bufferSize, UINT32 numChannels);
		static void GetSquareWave(float* buffer, UINT32 bufferSize, UINT32 numChannels);
	private:
		static float ConvertRadians(float degrees);
	};
}