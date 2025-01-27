#include "bbpch.h"

#include "RecorderManager.h"
namespace BackBeat {

	RecorderManager::RecorderManager()
		:
		m_Recording(false),
		m_Init(false),
		m_TimeEclipsed(0.0f),
		m_AudioRecorder(nullptr),
		m_DeviceRecorder(nullptr),
		m_ActiveID(UUID(0u))
	{

	}

	RecorderManager::~RecorderManager()
	{
		Stop();
	}

	void RecorderManager::Init(Recorder* recorder, DeviceRecorder* deviceRecorder)
	{
		if (m_Init)
			return;
		if (!recorder || !deviceRecorder)
			return;
		if (recorder->GetType() != RecorderType::audio || deviceRecorder->GetType() != RecorderType::device)
			return;

		m_AudioRecorder = recorder;
		m_DeviceRecorder = deviceRecorder;
		m_Init = true;
	}

	void RecorderManager::Start()
	{
		if (!m_Init)
			return;
		if (!Contains(m_ActiveID))
			return;

		if (ContainsAudio(m_ActiveID))
		{
			std::function<void()> callback = std::bind(&Recorder::Start, m_AudioRecorder);
			m_Thread.Start(callback, 0, true, false);
		}
		else
		{
			std::function<void()> callback = std::bind(&Recorder::Start, m_DeviceRecorder);
			m_Thread.Start(callback, 0, true, false);
		}

		m_TimeEclipsed = 0.0f;
		m_Timer.Start();
		m_Recording = true;
	}

	void RecorderManager::Stop()
	{
		if (!m_Init)
			return;
		if (!Contains(m_ActiveID))
			return;

		if (ContainsAudio(m_ActiveID))
			m_AudioRecorder->Stop();
		else
			m_DeviceRecorder->Stop();

		m_TimeEclipsed = 0.0f;
		m_Thread.Stop();
		m_Recording = false;
	}

	std::shared_ptr<MappedTrack> RecorderManager::AddRecordingMappedTrack(UUID id, RecorderType type)
	{
		if (!m_Init)
			return nullptr;
		if (Contains(id))
			return nullptr;
		if (type == RecorderType::none)
			return nullptr;

		std::shared_ptr<MappedTrack> track = nullptr;
		std::shared_ptr<Recording> recording = nullptr;

		if (type == RecorderType::audio)
		{
			track = TrackFactory::BuildMappedTempTrack(id, m_AudioRecorder->GetProps());
			recording = std::make_shared<Recording>(track);
			m_AudioRecordings[id] = recording;
		}
		else
		{
			track = TrackFactory::BuildMappedTempTrack(id, m_DeviceRecorder->GetProps());
			recording = std::make_shared<Recording>(track);
			m_DeviceRecordings[id] = recording;
		}

		return track;
	}

	void RecorderManager::SetRecorderActive(UUID id)
	{
		SetRecordingTrack(id);
	}

	void RecorderManager::SetRecorderInactive(UUID id)
	{
		if (!m_Init)
			return;
		if (m_Recording)
			return;
		if (!Contains(id))
			return;
		if (m_ActiveID != id)
			return;

		m_TimeEclipsed = 0.0f;
		m_ActiveID = UUID(0);
	}

	void RecorderManager::SetRecordingTrack(UUID id)
	{
		if (!m_Init)
			return;
		if (m_Recording)
			return;
		if (!Contains(id))
			return;

		std::shared_ptr<Recording> recording = nullptr;
		m_TimeEclipsed = 0.0f;

		if (ContainsAudio(id))
		{
			recording = m_AudioRecordings.at(id);
			m_AudioRecorder->SetRecording(recording);
		}
		else
		{
			recording = m_DeviceRecordings.at(id);
			m_DeviceRecorder->SetRecording(recording);
		}

		m_ActiveID = id;
	}

	void RecorderManager::SetDeviceRecorderIndex(UUID id, unsigned int index)
	{
		if (!m_Init)
			return;
		if (m_Recording)
			return;
		if (!ContainsDevice(id))
			return;

		std::shared_ptr<Recording> recording = m_DeviceRecordings.at(id);

		if (!recording)
			return;

		recording->SetIndex(index);
	}

	void RecorderManager::ClearTrack(UUID id)
	{
		if (!m_Init)
			return;
		if (m_Recording)
			return;
		if (!Contains(id))
			return;

		m_TimeEclipsed = 0.0f;

		if (ContainsAudio(id))
			m_AudioRecordings.at(id)->ClearTrack();
		else
			m_DeviceRecordings.at(id)->ClearTrack();
	}

	void RecorderManager::DeleteTrack(UUID id)
	{ 
		if (!m_Init)
			return;
		if (m_Recording)
			return;
		if (!Contains(id))
			return;

		if (ContainsAudio(id))
		{
			if (m_ActiveID == id)
				m_AudioRecorder->ClearTrack();
			m_AudioRecordings.erase(id);
		}
		else
		{
			if (m_ActiveID == id)
				m_DeviceRecorder->ClearTrack();
			m_DeviceRecordings.erase(id);
		}

		if (m_ActiveID == id)
			m_ActiveID = UUID(0);
	}

	void RecorderManager::ResetRecording(UUID id)
	{
		if (!m_Init)
			return;
		if (m_Recording)
			return;
		if (!Contains(id))
			return;

		if (m_ActiveID == id)
		{
			Stop();
			if (ContainsAudio(id))
				m_AudioRecorder->Reset();
			else
				m_DeviceRecorder->Reset();
		}
		else if (ContainsAudio(id))
			m_AudioRecordings.at(id)->Reset();
		else
			m_DeviceRecordings.at(id)->Reset();
	}

	void RecorderManager::ResetRecording(UUID id, AudioProps props)
	{
		if (!m_Init)
			return;
		if (m_Recording)
			return;
		if (!Contains(id))
			return;

		if (m_ActiveID == id)
		{
			Stop();
			if (ContainsAudio(id))
				m_AudioRecorder->Reset(props);
			else
				m_DeviceRecorder->Reset(props);
		}
		else if (ContainsAudio(id))
			m_AudioRecordings.at(id)->Reset(props);
		else
			m_DeviceRecordings.at(id)->Reset(props);
	}

	TimeMinSec RecorderManager::GetTime()
	{
		TimeMinSec time = {
			.minutes = 0,
			.seconds = 0,
			.milliseconds = 0
		};

		if (m_Recording)
			return Audio::GetTime(m_Timer.GetTime() + m_TimeEclipsed);
		else
			return Audio::GetTime(m_TimeEclipsed);
	}

	TimeMinSec RecorderManager::GetTimeSeconds()
	{
		TimeMinSec time = {
			.minutes = 0,
			.seconds = 0,
			.milliseconds = 0
		};

		if (m_Recording)
			time.seconds = unsigned int(m_Timer.GetTime() + m_TimeEclipsed);
		else
			time.seconds = unsigned int(m_TimeEclipsed);

		return time;
	}

	TimeMinSec RecorderManager::GetTimeMs()
	{
		TimeMinSec time = {
			.minutes = 0,
			.seconds = 0,
			.milliseconds = 0
		};

		if (m_Recording)
			return Audio::GetTimeMs(m_Timer.GetTime() + m_TimeEclipsed);
		else
			return Audio::GetTimeMs(m_TimeEclipsed);
	}

}