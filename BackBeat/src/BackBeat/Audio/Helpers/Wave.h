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
		void GetSinWave(std::shared_ptr<float[]> buffer, unsigned int bufferSize, unsigned int numChannels);
		void GetSawtoothUpWave(std::shared_ptr<float[]>  buffer, unsigned int bufferSize, unsigned int numChannels);
		void GetSawtoothDownWave(std::shared_ptr<float[]> buffer, unsigned int bufferSize, unsigned int numChannels);
		void GetTriangleWave(std::shared_ptr<float[]> buffer, unsigned int bufferSize, unsigned int numChannels);
		void GetSquareWave(std::shared_ptr<float[]> buffer, unsigned int bufferSize, unsigned int numChannels);
		float ConvertRadians(float degrees);
	}

}