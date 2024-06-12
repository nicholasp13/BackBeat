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

	namespace Wave {
		void GetSinWave(float* buffer, unsigned int bufferSize, unsigned int numChannels);
		void GetSawtoothUpWave(float* buffer, unsigned int bufferSize, unsigned int numChannels);
		void GetSawtoothDownWave(float* buffer, unsigned int bufferSize, unsigned int numChannels);
		void GetTriangleWave(float* buffer, unsigned int bufferSize, unsigned int numChannels);
		void GetSquareWave(float* buffer, unsigned int bufferSize, unsigned int numChannels);
		float ConvertRadians(float degrees);
	}

}