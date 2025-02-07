#include "bbpch.h"

#include <cmath>

#include "BackBeat/Audio/Instruments/Synth/SynthBase.h"
#include "Wave.h"
namespace BackBeat {

	void Wave::GetSinWave(std::shared_ptr<float[]>  buffer, unsigned int bufferSize, unsigned int numChannels)
	{
		float samplesIncrement = ConvertRadians(SynthBase::CircleDegrees / (float)bufferSize);

		for (unsigned int i = 0; i < bufferSize; i += numChannels) 
		{
			for (unsigned int j = 0; j < numChannels; j++)
			{
				unsigned int index = i + j;
				buffer[index] = sin(i * samplesIncrement);
			}
		}
	}

	void Wave::GetSawtoothUpWave(std::shared_ptr<float[]> buffer, unsigned int bufferSize, unsigned int numChannels)
	{
		float samplesIncrement = 2.0f / float(bufferSize);

		for (unsigned int i = 0; i < bufferSize; i += numChannels) 
		{
			for (unsigned int j = 0; j < numChannels; j++)
			{
				unsigned int index = i + j;
				buffer[index] = i * samplesIncrement - 1.0f;
			}
		}
	}

	void Wave::GetSawtoothDownWave(std::shared_ptr<float[]> buffer, unsigned int bufferSize, unsigned int numChannels)
	{
		float samplesIncrement = 2.0f / float(bufferSize);

		for (unsigned int i = 0; i < bufferSize; i += numChannels) 
		{
			for (unsigned int j = 0; j < numChannels; j++)
			{
				unsigned int index = i + j;
				buffer[index] = 1.0f - (i * samplesIncrement);
			}
		}
	}

	void Wave::GetTriangleWave(std::shared_ptr<float[]> buffer, unsigned int bufferSize, unsigned int numChannels)
	{
		float samplesIncrement = 4.0f / (float)bufferSize;
		float data = 0.0f;

		for (unsigned int i = 0; i < bufferSize; i += numChannels) 
		{
			if (i < bufferSize / 4 || i > bufferSize * 3 / 4)
				data += samplesIncrement;
			else
				data -= samplesIncrement;
			for (unsigned int j = 0; j < numChannels; j++) 
			{
				unsigned int index = i + j;
				buffer[index] = data;
			}
		}
	}

	void Wave::GetSquareWave(std::shared_ptr<float[]> buffer, unsigned int bufferSize, unsigned int numChannels)
	{
		for (unsigned int i = 0; i < bufferSize; i += numChannels) {
			for (unsigned int j = 0; j < numChannels; j++) {
				unsigned int index = i + j;
				if (i < bufferSize / 2)
					buffer[index] = 1.0f;
				else
					buffer[index] = -1.0f;
			}
		}
	}

	void Wave::GetSquareWave(std::shared_ptr<float[]> buffer, unsigned int bufferSize, unsigned int numChannels, float dutyCycle)
	{
		unsigned int activeNum = 0;

		if (dutyCycle < 0.0f)
			activeNum = 0;
		else if (dutyCycle >= 1.0f)
			activeNum = bufferSize;
		else
			activeNum = unsigned int((float)bufferSize * dutyCycle);

		for (unsigned int i = 0; i < bufferSize; i += numChannels) {
			for (unsigned int j = 0; j < numChannels; j++) {
				unsigned int index = i + j;
				if (i < activeNum)
					buffer[index] = 1.0f;
				else
					buffer[index] = -1.0f;
			}
		}
	}
	
	// Might want this in another file for greater access or change into a static function if not needed anywhere else
	float Wave::ConvertRadians(float degrees)
	{
		return (degrees * SynthBase::Pi) / 180.0f;
	}

}