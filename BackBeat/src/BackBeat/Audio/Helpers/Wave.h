#pragma once

// TODO:
//     - Add more waveforms (i.e. use FFT of samples to find wave forms that make up unique sounds and create functions
//     that create those sounds using a sum of those waves)
//     - Find better folder for this static class
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