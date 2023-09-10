#include "bbpch.h"

#include <cmath>

#include "BackBeat/Audio/Audio.h"
#include "BackBeat/Core/Core.h"
#include "SinWave.h"

// TODO:
// * Change into basic Wave static class that creates different wave types: SAWTOOTH (up/down), SQUARE, TRIANGLE
// * Channels will be allocated at a higher object level
// Static class to return a Sin Wave. 
namespace BackBeat {

	// Creates One Sin Wave per buffer
	void SinWave::GetSinWave(float* buffer, UINT32 bufferSize, UINT32 numChannels, float amp)
	{
		float samplesIncrement = ConvertRadians(CIRCLE_DEGREES / (float)bufferSize);

		for (UINT32 i = 0; i < bufferSize; i += numChannels) {
			for (UINT32 j = 0; j < numChannels; j++)
				buffer[i + j] = sin(i * samplesIncrement) * amp;
		}
	}

	float SinWave::ConvertRadians(float degrees)
	{
		return (degrees * PI) / 180;
	}
}