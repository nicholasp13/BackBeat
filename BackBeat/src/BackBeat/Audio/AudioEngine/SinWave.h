#pragma once

// TODO:
// Change into basic Wave static class that creates different wave types: SAWTOOTH (up/down), SQUARE, TRIANGLE
namespace BackBeat {

	class SinWave
	{
	public:
		SinWave() {};
		~SinWave() {};

		static void GetSinWave(float* buffer, UINT32 bufferSize, UINT32 numChannels, float amp);

	private:
		static float ConvertRadians(float degrees);
	};
}