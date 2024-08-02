#include "bbpch.h"

#include "RecorderManager.h"
namespace BackBeat {

	RecorderManager::RecorderManager()
		: m_Recording(false)
	{

	}

	RecorderManager::~RecorderManager()
	{

	}

	// TODO: Make inline if not expanded on
	void RecorderManager::Start()
	{
		m_Recording = true;
	}

	// TODO: Make inline if not expanded on
	void RecorderManager::Stop()
	{
		m_Recording = false;
	}

	// TODO: Make inline if not expanded on
	void RecorderManager::AddRecorder(UUID id, AudioProps props)
	{
		m_Recorders[id] = RecorderBuilder::BuildRecorder(id, props);
	}

	std::shared_ptr<Recorder> RecorderManager::GetRecorder(UUID id)
	{
		if (m_Recorders.find(id) == m_Recorders.end())
			return nullptr;

		return m_Recorders.at(id);
	}

	void RecorderManager::Record(UUID id, void* buffer, unsigned int numSamples)
	{
		if (!m_Recording)
			return;
		if (m_Recorders.find(id) == m_Recorders.end())
			return;
	
		std::shared_ptr<Recorder> recorder = m_Recorders.at(id);
		if (!recorder->IsRecording() || recorder->GetType() != RecorderType::audio)
			return;

		auto data = reinterpret_cast<char*>(buffer);
		recorder->Record(data, numSamples);
	}

}