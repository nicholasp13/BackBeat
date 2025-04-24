#pragma once

#include "BackBeat/Audio/PlayBack/MappedTrack.h"
namespace BackBeat {

	namespace ZeroCrossingFinder {

		static inline unsigned int FindZeroCrossing(float* buffer, unsigned int numSamples, unsigned int numChannels, unsigned int startIndex, bool forward)
		{
			if (!buffer)
				return startIndex;
			if (numSamples <= 1)
				return startIndex;
			if (startIndex > numSamples)
				return startIndex;

			int currIndex = (int)startIndex;
			int prevIndex = currIndex;

			if (forward)
				currIndex += numChannels;
			else
				currIndex -= numChannels;

			// MONO vals
			float currVal = 0.0f;
			float prevVal = 0.0f;
			// STEREO vals
			float currVal1 = 0.0f;
			float currVal2 = 0.0f;
			float prevVa11 = 0.0f;
			float prevVal2 = 0.0f;

			while (currIndex >= 0 && currIndex < (int)numSamples)
			{
				switch (numChannels)
				{
					case (Audio::Mono):
						currVal = buffer[currIndex];
						prevVal = buffer[prevIndex];

						if (currVal * prevVal <= 0.0f)
							return (unsigned int)currIndex;

						if (forward)
						{
							currIndex++;
							prevIndex++;
						}
						else
						{
							currIndex--;
							prevIndex--;
						}

						break;

					case (Audio::Stereo):
						currVal1 = buffer[currIndex];
						currVal2 = buffer[currIndex + 1];
						prevVa11 = buffer[prevIndex];
						prevVal2 = buffer[prevIndex + 1];

						if ((currVal1 * prevVa11 <= 0.0f) && (currVal2 * prevVal2 <= 0.0f))
							return (unsigned int)currIndex;

						if (forward)
						{
							currIndex += Audio::Stereo;
							prevIndex += Audio::Stereo;
						}
						else
						{
							currIndex -= Audio::Stereo;
							prevIndex -= Audio::Stereo;
						}

						break;


					default:
						return startIndex;
				}
			}

			return startIndex;
		}

		// @params:
		// - track        = MappedTrack to load from
		// - startIndexx  = index IN BYTES where the finding starts
		// - forward      = bool that decides the direction where from the startIndex to go positive(true) / negative(false)
		// - seconds      = how many seconds max from the startIndex forward or backwards the finder will search
		static inline unsigned int FindZeroCrossing(std::shared_ptr<MappedTrack> track, unsigned int startIndex, bool forward, unsigned int seconds)
		{
			if (!track)
				return startIndex;

			AudioProps props = track->GetProps();
			unsigned int size = track->GetSize();
			unsigned int numBytes = seconds * props.byteRate;

			if (forward)
				numBytes = (startIndex + numBytes > size) ? size - startIndex : numBytes;
			else
				numBytes = (int(startIndex) - int(numBytes) < 0) ? startIndex : numBytes;

			unsigned int returnIndex = startIndex;
			numBytes = numBytes - (numBytes % props.blockAlign);
			unsigned int numSamples = numBytes / props.blockAlign * props.numChannels;
			float* buffer = new float[numSamples];

			if (forward)
			{
				unsigned int position = startIndex - (startIndex % props.blockAlign);
				track->Read((byte*)buffer, numBytes, position);

				unsigned int offset = FindZeroCrossing(buffer, numSamples, props.numChannels, 0, forward);
				returnIndex += (offset * props.blockAlign / props.numChannels);
			}
			else
			{
				unsigned int position = (startIndex - numBytes) - (startIndex % props.blockAlign);
				track->Read((byte*)buffer, numBytes, position);

				unsigned int offset = FindZeroCrossing(buffer, numSamples, props.numChannels, numSamples - 1, forward);
				returnIndex -= ((numSamples - 1 - offset ) * props.blockAlign / props.numChannels);
			}

			delete[] buffer;
			return returnIndex;
		}

	}

}