#include "bbpch.h"

#include "AudioRecorder.h"
namespace BackBeat {

	AudioRecorder::AudioRecorder()
		:
		m_IsRecording(false),
		m_Init(false),
		m_BufferSize(0u),
		m_AudioProps(AudioProps()),
		m_Recording(m_ID.ToString(), AudioProps()),
		m_Sink(nullptr)
	{

	}

	AudioRecorder::~AudioRecorder()
	{
		Stop();
	}

	void AudioRecorder::Start()
	{
		if (!m_Init)
			return;

		m_IsRecording = true;
		Record();
	}

	void AudioRecorder::Stop()
	{
		m_IsRecording = false;
	}

	void AudioRecorder::Reset()
	{
		if (!m_IsRecording)
			m_Recording.Reset();
	}

	void AudioRecorder::Reset(AudioProps props)
	{
		if (!m_IsRecording)
			m_Recording.Reset(props);
	}

	bool AudioRecorder::SaveWAV(std::string filePath)
	{
		if (!m_IsRecording)
			return m_Recording.SaveWAV(filePath);
		return false;
	}
	
	std::shared_ptr<Track> AudioRecorder::GetRecordingTrack()
	{
		if (!m_IsRecording)
			return m_Recording.GetTrack();
		return nullptr;
	}

	void AudioRecorder::Init(AudioProps props, AudioSink* sink)
	{
		if (m_Init)
			return;
		if (!sink)
			return;
		if (props.bitDepth != Audio::FloatBitSize)
			return;

		m_AudioProps = props;
		m_Sink = sink;
		// ~1ms bufferSize
		m_BufferSize = props.sampleRate * props.numChannels / 1000;
		if (m_BufferSize > ActualBufferSize)
			m_BufferSize = ActualBufferSize;

		m_Recording.Reset(m_AudioProps);
		m_Init = true;

	}

	void AudioRecorder::Record()
	{
		// NOTE: Sleeping for buffsize while making sense, makes the recorder too slow for the renderer.
		//       Through some testing this amount of sleep reduces CPU usage a decent amount however this testing
		//       was not thorough and the sleepTime is subject to change if need be
		const auto sleepTime = std::chrono::nanoseconds(10);
		auto readBuffer = m_Buffer;
		auto writeBuffer = reinterpret_cast<char*>(m_Buffer);
		unsigned int numFrames = m_BufferSize / m_AudioProps.numChannels;
		float defaultVal = 0.0f;
		Audio::FlushBufferT(readBuffer, &defaultVal, m_BufferSize);

		m_Sink->Reset();
		while (m_IsRecording)
		{
			if (m_Sink->ReadData(readBuffer, m_BufferSize))
				m_Recording.Record(writeBuffer, numFrames);
			std::this_thread::sleep_for(sleepTime);
		}
	}
}