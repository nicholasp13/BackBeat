#include "bbpch.h"

#include "Visualizer.h"
namespace BackBeat {

	Visualizer::Visualizer()
		:
		m_On(false),
		m_UpdateBufferSize(0u), 
		m_ChannelBufferSize(0u),
		m_ChannelUpdateSize(0u),
		m_Props(AudioProps()),
		m_Sink(nullptr),
		m_InputBuffer(new float[s_ArraySize]),
		m_OutputBuffer(new float[s_ArraySize])
	{

	}

	Visualizer::~Visualizer()
	{

	}

	void Visualizer::Init(AudioProps props, AudioSink* sink, unsigned int updateBufferSize)
	{
		if (!sink)
			return;
		if (updateBufferSize == 0 || updateBufferSize > s_ArraySize)
			return;

		m_Props = props;
		m_Sink = sink;
		m_UpdateBufferSize = updateBufferSize;
		m_ChannelBufferSize = s_ArraySize / props.numChannels;
		m_ChannelUpdateSize = updateBufferSize / props.numChannels;

		float defaultVal = 0.0f;
		for (unsigned int i = 0; i < props.numChannels; i++)
		{
			m_ChannelBuffers.push_back(std::shared_ptr<float[]>(new float[m_ChannelBufferSize]));
			Audio::FlushBufferT(m_ChannelBuffers[i].get(), &defaultVal, m_ChannelBufferSize);
		}

		Audio::FlushBufferT(m_OutputBuffer.get(), &defaultVal, s_ArraySize);
		Audio::FlushBufferT(m_InputBuffer.get(), &defaultVal, s_ArraySize);
	}

	void Visualizer::Update()
	{
		if (!m_On)
			return;
		if (!m_Sink)
			return;
		if (!m_Sink->ReadData(m_InputBuffer.get(), m_UpdateBufferSize))
			return;

		std::memmove(m_OutputBuffer.get() + m_UpdateBufferSize, m_OutputBuffer.get(), size_t((s_ArraySize - m_UpdateBufferSize) * Audio::FloatByteSize));
		std::memcpy(m_OutputBuffer.get(), m_InputBuffer.get(), size_t(m_UpdateBufferSize * Audio::FloatByteSize));

		for (unsigned int i = 0; i < m_Props.numChannels; i++)
			std::memmove(m_ChannelBuffers[i].get() + m_ChannelUpdateSize, m_ChannelBuffers[i].get(), size_t((m_ChannelBufferSize - m_ChannelUpdateSize) * Audio::FloatByteSize));

		unsigned int channelIndex = 0;
		for (unsigned int i = 0; i < m_UpdateBufferSize; i++)
		{
			m_ChannelBuffers[i % m_Props.numChannels][channelIndex] = m_OutputBuffer[i];
			if ((i % m_Props.numChannels) == 1)
				channelIndex++;
		}

		return;
	}

	void Visualizer::On()
	{
		m_Sink->Reset();
		m_On = true;
	}

	void Visualizer::Off()
	{
		m_On = false;
		float defaultVal = 0.0f;
		Audio::FlushBufferT(m_OutputBuffer.get(), &defaultVal, s_ArraySize);
		Audio::FlushBufferT(m_InputBuffer.get(), &defaultVal, s_ArraySize);
		
		for (unsigned int i = 0; i < m_Props.numChannels; i++)
			Audio::FlushBufferT(m_ChannelBuffers[i].get(), &defaultVal, m_ChannelBufferSize);
	}

	float* Visualizer::GetChannelBuffer(unsigned int index)
	{
		if (index > m_Props.numChannels)
			return nullptr;

		return m_ChannelBuffers[index].get();
	}

}