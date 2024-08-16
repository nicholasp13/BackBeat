#include "bbpch.h"

#include <cmath>

#include "BackBeat/Audio/Instruments/Synth/SynthBase.h"
#include "Wave.h"
namespace BackBeat {

	void Wave::GetSinWave(float* buffer, unsigned int bufferSize, unsigned int numChannels)
	{
		float samplesIncrement = ConvertRadians(SynthBase::CircleDegrees / (float)bufferSize);

		for (unsigned int i = 0; i < bufferSize; i += numChannels) {
			for (unsigned int j = 0; j < numChannels; j++)
				buffer[i + j] = sin(i * samplesIncrement);
		}
	}

	void Wave::GetSawtoothUpWave(float* buffer, unsigned int bufferSize, unsigned int numChannels)
	{
		float samplesIncrement = 2.0f / float(bufferSize);

		for (unsigned int i = 0; i < bufferSize; i += numChannels) {
			for (unsigned int j = 0; j < numChannels; j++)
				buffer[i + j] = i * samplesIncrement - 1.0f;
		}
	}

	void Wave::GetSawtoothDownWave(float* buffer, unsigned int bufferSize, unsigned int numChannels)
	{
		float samplesIncrement = 2.0f / float(bufferSize);

		for (unsigned int i = 0; i < bufferSize; i += numChannels) {
			for (unsigned int j = 0; j < numChannels; j++)
				buffer[i + j] = 1.0f - (i * samplesIncrement);
		}
	}

	void Wave::GetTriangleWave(float* buffer, unsigned int bufferSize, unsigned int numChannels)
	{
		float samplesIncrement = 4.0f / (float)bufferSize;
		float data = 0.0f;

		for (unsigned int i = 0; i < bufferSize; i += numChannels) {
			for (unsigned int j = 0; j < numChannels; j++) {
				if (i < bufferSize / 4 || i > bufferSize * 3 / 4)
					data += samplesIncrement;
				else
					data -= samplesIncrement;
				buffer[i] = data;
				buffer[i + 1] = data;
			}
		}
	}

	void Wave::GetSquareWave(float* buffer, unsigned int bufferSize, unsigned int numChannels)
	{
		for (unsigned int i = 0; i < bufferSize; i += numChannels) {
			for (unsigned int j = 0; j < numChannels; j++) {
				if (i < bufferSize / 2)
					buffer[i + j] = 1.0f;
				else
					buffer[i + j] = -1.0f;
			}
		}
	}
	
	// NOTE: Might want this in another file for greater access or change into a static function if not needed anywhere else
	float Wave::ConvertRadians(float degrees)
	{
		return (degrees * SynthBase::Pi) / 180;
	}

}