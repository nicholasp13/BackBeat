#include "bbpch.h"

#include "SplicerPlayerProcessor.h"
namespace BackBeat {


	SplicerPlayerProcessor::SplicerPlayerProcessor()
		:
		m_On(false),
		m_Looping(false),
		m_BufferSize(0),
		m_Position(0),
		m_Props(AudioProps()),
		m_InputBuffer(nullptr)
	{

	}

	SplicerPlayerProcessor::~SplicerPlayerProcessor()
	{
		if (m_InputBuffer)
			delete[m_BufferSize] m_InputBuffer;
	}

	void SplicerPlayerProcessor::On()
	{
		if (m_InputBuffer)
			m_On = true;
	}

	void SplicerPlayerProcessor::Play(float* data, unsigned int numFrames, AudioProps props)
	{
		if (!data)
			return;
		
		if (m_InputBuffer)
			delete[m_BufferSize] m_InputBuffer;
		m_Position = 0;
		m_InputBuffer = data;
		m_BufferSize = numFrames * props.numChannels;
		m_Props = props;
	}

	void SplicerPlayerProcessor::ProcessSamples(unsigned int numFrames, unsigned int sampleRate, unsigned int numChannels)
	{
		if (!m_On)
			return;
		if (!m_InputBuffer)
			return;
		if (numFrames == 0)
			return;

		// This should always be false as Splicer and its playback objects should always match mixer's props
		if (m_Props.numChannels != numChannels && m_Props.sampleRate != sampleRate)
		{
			BB_CORE_ERROR("SPLICER PLAYBACK AUDIO PROPS DO NOT MATCH MIXERS!");
			m_On = false;
		}

		unsigned int numSamples = numFrames * numChannels;
		unsigned int numBytes = numSamples * m_Props.bitDepth / Audio::ByteBitSize;
		Audio::FlushBuffer((byte*)m_OutputBuffer, numBytes);

		unsigned int samplesLeft = 0;
		if (m_Position < m_BufferSize)
			samplesLeft = m_BufferSize - m_Position;
		else
		{
			samplesLeft = m_BufferSize;
			m_Position = 0;
			if (!m_Looping)
			{
				m_On = false;
				return;
			}
		}

		float* inputBufferPtr = m_InputBuffer + m_Position;
		if (numSamples < samplesLeft)
		{
			Audio::CopyInputToOutput(inputBufferPtr, m_OutputBuffer, numBytes);
			m_Position += numSamples;
		}
		else
		{
			numBytes = samplesLeft * m_Props.bitDepth / Audio::ByteBitSize;
			Audio::CopyInputToOutput(inputBufferPtr, m_OutputBuffer, numBytes);
			if (m_Looping)
			{
				unsigned int samplesLeftOver = numSamples - samplesLeft;
				float* outputBufferPtr = m_OutputBuffer + samplesLeft;
				unsigned int numBytesLeftOver = samplesLeftOver * m_Props.blockAlign;
				Audio::CopyInputToOutput(m_InputBuffer, outputBufferPtr, numBytesLeftOver);
				m_Position = samplesLeftOver;
			}
			else
			{
				m_Position = 0;
				m_On = false;
			}
		}

	}

	void SplicerPlayerProcessor::Clear()
	{
		m_On = false;
		m_Looping = false;
		m_Position = 0;
		m_InputBuffer = nullptr;
	}

}