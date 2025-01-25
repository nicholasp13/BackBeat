#include "bbpch.h"

#include "AudioFileLoader.h"
namespace BackBeat {

	AudioFileLoader::AudioFileLoader(unsigned int bufferSize)
		: m_BufferSize(bufferSize), m_Buffer(std::make_shared<byte[]>(bufferSize))
	{

	}

	AudioFileLoader::~AudioFileLoader()
	{

	}

	// @params:
	// - track        - MappedTrack to load from
	// - output       - buffer to output data to
	// - numBytes     - totalBytes to load into the output buffer
	// - channelIndex - which channel (if audio is multi-channeled) to load the data to, starting from 0,
	//                  -1 means load all data into output
	bool AudioFileLoader::Load(std::shared_ptr<MappedTrack> track, byte* output, 
		unsigned int numBytes, unsigned int position, int channelIndex)
	{
		if (!track)
			return false;

		AudioProps props = track->GetProps();

		if (channelIndex >= props.numChannels)
			return false;
		if (numBytes * props.numChannels > m_BufferSize)
			return false;
		if ((numBytes % props.blockAlign) != 0)
			return false;
		if ((position % props.blockAlign) != 0)
			return false;

		unsigned int bytesToLoad = 0;
		if (channelIndex == -1 || props.numChannels == 1)
			bytesToLoad = numBytes;
		else
			bytesToLoad = numBytes * props.numChannels;

		Audio::FlushBuffer((byte*)m_Buffer.get(), m_BufferSize);

		if (!track->Read(m_Buffer.get(), numBytes * props.numChannels, position))
			return false;

		if (channelIndex == -1 || props.numChannels == 1)
		{
			Audio::CopyInputToOutput(m_Buffer.get(), output, bytesToLoad);
			return true;
		}
		else
		{
			unsigned int byteSize = 0;

			switch (props.bitDepth)
			{

			case (Audio::ByteBitSize):
			{
				byteSize = Audio::ByteByteSize;
				break;
			}

			case (Audio::Int16BitSize):
			{
				byteSize = Audio::Int16ByteSize;
				break;
			}

			case (Audio::Int24BitSize):
			{
				byteSize = Audio::Int24ByteSize;
				break;
			}

			case (Audio::FloatBitSize):
			{
				byteSize = Audio::FloatByteSize;
				break;
			}

			case (Audio::DoubleBitSize):
			{
				byteSize = Audio::DoubleByteSize;
				break;
			}

			default:
			{
				BB_CORE_ERROR("Unsupported bit depth");
				return false;
			}

			}

			unsigned int inputPosition = 0;
			for (unsigned int i = 0; i < numBytes; i += byteSize)
			{
				for (unsigned int j = 0; j < props.numChannels; j++)
				{
					if (j == channelIndex)
						memcpy(output + i, m_Buffer.get() + inputPosition, (size_t)byteSize);

					inputPosition += byteSize;
				}
			}
		}
		return true;
	}
}