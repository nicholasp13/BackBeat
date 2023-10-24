#pragma once

// TODO:
// Change into basic Wave static class that creates different wave types: SAWTOOTH (up/down), SQUARE, TRIANGLE
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
		static void GetSinWave(float* buffer, UINT32 bufferSize, UINT32 numChannels, float amp);
		static void GetSawtoothUpWave(float* buffer, UINT32 bufferSize, UINT32 numChannels, float amp);
		static void GetSawtoothDownWave(float* buffer, UINT32 bufferSize, UINT32 numChannels, float amp);
		static void GetTriangleWave(float* buffer, UINT32 bufferSize, UINT32 numChannels, float amp);
		static void GetSquareWave(float* buffer, UINT32 bufferSize, UINT32 numChannels, float amp);
	private:
		static float ConvertRadians(float degrees);
	};
}