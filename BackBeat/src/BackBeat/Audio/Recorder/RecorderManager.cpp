#include "bbpch.h"

#include "RecorderManager.h"
namespace BackBeat {

	RecorderManager::RecorderManager()
		:
		m_Recording(false),
		m_Init(false),
		m_AudioRecorder(nullptr),
		m_DeviceRecorder(nullptr),
		m_ActiveID(UUID(0u))
	{

	}

	RecorderManager::~RecorderManager()
	{

	}

	void RecorderManager::Init(Recorder* recorder, Recorder* deviceRecorder)
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
			m_AudioRecorder->Start();
		else
			m_DeviceRecorder->Start();
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
		m_Recording = false;
	}

	std::shared_ptr<Track> RecorderManager::AddRecordingTrack(UUID id, RecorderType type)
	{
		if (!m_Init)
			return nullptr;
		if (Contains(id))
			return nullptr;
		if (type == RecorderType::none)
			return nullptr;

		std::shared_ptr<Track> track = nullptr;
		if (type == RecorderType::audio)
		{
			track = TrackFactory::BuildTempTrack(id, m_AudioRecorder->GetProps());
			m_AudioRecordings[id] = track;
		}
		else
		{
			track = TrackFactory::BuildTempTrack(id, m_DeviceRecorder->GetProps());
			m_DeviceRecordings[id] = track;
		}

		return track;
	}

	void RecorderManager::AddRecordingTrack(UUID id, std::shared_ptr<Track> track, RecorderType type)
	{
		if (!m_Init)
			return;
		if (Contains(id))
			return;
		if (!track)
			return;
		if (type == RecorderType::none)
			return;

		if (type == RecorderType::audio)
			m_AudioRecordings[id] = track;
		else
			m_DeviceRecordings[id] = track;
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

		m_AudioRecorder->ClearTrack();
		m_DeviceRecorder->ClearTrack();
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

		std::shared_ptr<Track> track = nullptr;
		if (ContainsAudio(id))
		{
			track = m_AudioRecordings.at(id);
			m_AudioRecorder->SetRecordingTrack(track);
		}
		else
		{
			track = m_DeviceRecordings.at(id);
			m_DeviceRecorder->SetRecordingTrack(track);
		}
		m_ActiveID = id;
	}

	void RecorderManager::ClearTrack(UUID id)
	{
		if (!m_Init)
			return;
		if (m_Recording)
			return;
		if (!Contains(id))
			return;

		if (ContainsAudio(id))
			m_AudioRecorder->ClearTrack();
		else
			m_DeviceRecorder->ClearTrack();
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

		auto oldID = m_ActiveID;
		SetRecordingTrack(id);

		if (ContainsAudio(id))
			m_AudioRecorder->Reset();
		else
			m_DeviceRecorder->Reset();

		m_ActiveID = oldID;
	}

}