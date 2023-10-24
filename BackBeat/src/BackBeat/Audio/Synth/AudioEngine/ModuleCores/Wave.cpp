#include "bbpch.h"

#include <cmath>

#include "BackBeat/Audio/Synth/SynthBase.h"
#include "BackBeat/Audio/Audio.h"
#include "BackBeat/Core/Core.h"
#include "Wave.h"

// TODO:
// * Additional custom Waves for custom sounds
// Static class to return a Waveform.
namespace BackBeat {

	void Wave::GetSinWave(float* buffer, UINT32 bufferSize, UINT32 numChannels, float amp)
	{
		float samplesIncrement = ConvertRadians(CIRCLE_DEGREES / (float)bufferSize);

		for (UINT32 i = 0; i < bufferSize; i+= numChannels) {
			for (UINT32 j = 0; j < numChannels; j++)
				buffer[i + j] = sin(i * samplesIncrement) * amp;
		}
	}

	void Wave::GetSawtoothUpWave(float* buffer, UINT32 bufferSize, UINT32 numChannels, float amp)
	{
		float samplesIncrement = 2.0f * amp / float(bufferSize);

		for (UINT32 i = 0; i < bufferSize; i+= numChannels) {
			for (UINT32 j = 0; j < numChannels; j++)
				buffer[i + j] = i * samplesIncrement - amp;
		}
	}

	void Wave::GetSawtoothDownWave(float* buffer, UINT32 bufferSize, UINT32 numChannels, float amp)
	{
		float samplesIncrement = 2.0f * amp / float(bufferSize);

		for (UINT32 i = 0; i < bufferSize; i += numChannels) {
			for (UINT32 j = 0; j < numChannels; j++)
				buffer[i + j] = amp - (i * samplesIncrement);
		}
	}

	void Wave::GetTriangleWave(float* buffer, UINT32 bufferSize, UINT32 numChannels, float amp)
	{
		float samplesIncrement = 4.0f * amp / (float)bufferSize;
		float data = 0.0f;

		for (UINT32 i = 0; i < bufferSize; i += numChannels) {
			for (UINT32 j = 0; j < numChannels; j++) {
				if (i < bufferSize / 4 || i > bufferSize * 3 / 4)
					data += samplesIncrement;
				else
					data -= samplesIncrement;
				buffer[i + j] = data;
			}
		}
	}

	void Wave::GetSquareWave(float* buffer, UINT32 bufferSize, UINT32 numChannels, float amp)
	{
		for (UINT32 i = 0; i < bufferSize; i += numChannels) {
			for (UINT32 j = 0; j < numChannels; j++) {
				if (i < bufferSize / 2)
					buffer[i + j] = amp;
				else
					buffer[i + j] = amp * -1.0f;
			}
		}
	}

	float Wave::ConvertRadians(float degrees)
	{
		return (degrees * PI) / 180;
	}
}