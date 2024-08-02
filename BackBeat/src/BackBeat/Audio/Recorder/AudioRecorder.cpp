#include "bbpch.h"

#include "AudioRecorder.h"
namespace BackBeat {

	AudioRecorder::AudioRecorder(std::string tempFilePath, AudioProps props)
		:
		m_IsRecording(false),
		m_AudioProps(props),
		m_Recording(tempFilePath, props)
	{

	}

	AudioRecorder::~AudioRecorder()
	{
		
	}

	// TODO: Change to inline if no extra lines are added
	void AudioRecorder::Start()
	{
		m_IsRecording = true;
	}

	// TODO: Change to inline if no extra lines are added
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

	void AudioRecorder::Record(char* data, unsigned int numSamples)
	{
		if (!m_IsRecording)
			return;
		unsigned int numFrames = numSamples / m_AudioProps.numChannels;
		m_Recording.Record(data, numFrames);
	}

}