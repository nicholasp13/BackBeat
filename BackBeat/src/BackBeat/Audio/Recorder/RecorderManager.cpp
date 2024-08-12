#include "bbpch.h"

#include "RecorderManager.h"
namespace BackBeat {

	RecorderManager::RecorderManager()
		: m_Recording(false), m_ActiveID(UUID(0u)), m_ActiveDeviceTrackID(UUID(0u))
	{

	}

	RecorderManager::~RecorderManager()
	{

	}

	void RecorderManager::Start()
	{
		if (!m_DeviceRecorder)
			return;
		if (m_ActiveID == UUID(0))
			return;
		m_Recorders.at(m_ActiveID)->Start();
		m_Recording = true;
	}

	void RecorderManager::Stop()
	{
		m_Recording = false;
		if (!m_DeviceRecorder)
			return;
		if (m_ActiveID == UUID(0))
			return;
		m_Recorders.at(m_ActiveID)->Off();
		m_Recorders.at(m_ActiveID)->Stop();
	}

	std::shared_ptr<Recorder> RecorderManager::AddRecorder(UUID id, AudioProps props)
	{
		auto recorder = RecorderBuilder::BuildRecorder(id, props);
		m_Recorders[id] = recorder;
		return recorder;
	}

	void RecorderManager::AddDeviceRecorder(std::shared_ptr<DeviceRecorder> recorder)
	{
		if (m_DeviceRecorder)
			return;
		m_DeviceRecorder = recorder;
		m_Recorders[recorder->GetID()] = recorder;
	}

	std::shared_ptr<Recorder> RecorderManager::GetRecorder(UUID id)
	{
		if (!Contains(id))
			return nullptr;

		return m_Recorders.at(id);
	}

	void RecorderManager::Record(UUID id, void* buffer, unsigned int numSamples)
	{
		if (!m_Recording)
			return;
		if (m_ActiveID != id)
			return;

		std::shared_ptr<Recorder> recorder = m_Recorders.at(id);
		if (recorder->GetType() != RecorderType::audio)
			return;

		auto data = reinterpret_cast<char*>(buffer);
		recorder->Record(data, numSamples);
	}
	
	void RecorderManager::SetRecorderActive(UUID id)
	{
		if (m_ActiveID == id)
			return;
		if (!Contains(id))
			return;

		for (auto itr = m_Recorders.begin(); itr != m_Recorders.end(); itr++)
			itr->second->Off();

		m_Recorders.at(id)->On();
		m_ActiveID = id;
	}

	void RecorderManager::SetRecorderInactive(UUID id)
	{
		if (m_ActiveID != id)
			return;
		m_Recorders.at(id)->Off();
		m_Recorders.at(id)->Stop();
		m_ActiveID = UUID(0);
	}

	void RecorderManager::SetDeviceRecorderTrack(UUID trackID, std::shared_ptr<Track> track)
	{
		if (!track)
			return;
		m_DeviceRecorder->SetRecordingTrack(track);
		m_ActiveDeviceTrackID = trackID;
	}

	void RecorderManager::ClearDeviceTrack()
	{
		m_DeviceRecorder->ClearTrack();
		m_ActiveDeviceTrackID = UUID(0u);
	}

	void RecorderManager::DeleteRecorder(UUID id)
	{ 
		if (m_ActiveID == id)
			m_ActiveID = UUID(0);
		m_Recorders.at(id)->Off();
		m_Recorders.at(id)->Stop();
		m_Recorders.erase(id); 
	}
}