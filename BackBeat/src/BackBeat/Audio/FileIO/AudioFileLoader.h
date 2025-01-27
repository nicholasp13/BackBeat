#pragma once

#include "BackBeat/Audio/PlayBack/MappedTrack.h"
namespace BackBeat {

	class AudioFileLoader
	{
	public:
		AudioFileLoader(unsigned int bufferSize);
		~AudioFileLoader();

		bool Load(std::shared_ptr<MappedTrack> track, byte* output, unsigned int numBytes, unsigned int position,
			int channelIndex);
		bool Unload(std::shared_ptr<MappedTrack> track, byte* input, AudioProps inputProps, unsigned int numInputBytes, 
			int channelIndex);

	private:
		unsigned int m_BufferSize;
		std::shared_ptr<byte[]> m_Buffer;
	};

}