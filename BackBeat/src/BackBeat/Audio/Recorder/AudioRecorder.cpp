#include "bbpch.h"

#include "AudioRecorder.h"
namespace BackBeat {

	AudioRecorder::AudioRecorder(std::string tempFilePath, AudioProps props)
		:
		m_On(false),
		m_AudioProps(props),
		m_Recording(tempFilePath, props)
	{

	}

	AudioRecorder::~AudioRecorder()
	{
		
	}

	// TODO: Change to inline if no extra lines are added
	void AudioRecorder::On()
	{
		m_On = true;
	}

	// TODO: Change to inline if no extra lines are added
	void AudioRecorder::Off()
	{
		m_On = false;
	}

	void AudioRecorder::Reset()
	{
		if (!m_On)
			m_Recording.Reset();
	}

	void AudioRecorder::Reset(AudioProps props)
	{
		if (!m_On)
			m_Recording.Reset(props);
	}

	void AudioRecorder::Record(char* data, unsigned int numSamples)
	{
		if (!m_On)
			return;
		unsigned int numFrames = numSamples / m_AudioProps.numChannels;
		m_Recording.Record(data, numFrames);
	}

	bool AudioRecorder::SaveWAV(std::string filePath)
	{
		if (!m_On)
			return m_Recording.SaveWAV(filePath);
		return false;
	}

	std::shared_ptr<Track> AudioRecorder::GetRecordingTrack()
	{
		m_Recording.CreateTrack();
		return m_Recording.GetTrack();
	}

}