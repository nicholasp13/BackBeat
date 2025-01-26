#include "bbpch.h"

#include "PlayerManager.h"
namespace BackBeat {

	PlayerManager::PlayerManager()
		: m_Playing(false), m_TimeEclipsed(0.0f)
	{

	}

	PlayerManager::~PlayerManager()
	{
		StopAll();
	}

	void PlayerManager::StartAll()
	{
		for (auto it = m_Players.begin(); it != m_Players.end(); it++)
		{
			(*it)->Start();
		}
		m_Playing = true;
		m_TimeEclipsed = 0.0f;
		m_Timer.Start();
	}

	void PlayerManager::StopAll()
	{
		for (auto it = m_Players.begin(); it != m_Players.end(); it++)
		{
			(*it)->Stop();
		}
		m_TimeEclipsed += m_Timer.GetTime();
		m_Playing = false;
	}

	void PlayerManager::PlayAll()
	{
		for (auto it = m_Players.begin(); it != m_Players.end(); it++)
		{
			(*it)->Play();
		}
		m_Playing = true;
		m_Timer.Start();
	}

	void PlayerManager::PauseAll()
	{
		for (auto it = m_Players.begin(); it != m_Players.end(); it++)
		{
			(*it)->Pause();
		}
		m_TimeEclipsed += m_Timer.GetTime();
		m_Playing = false;
	}

	void PlayerManager::ResetAll()
	{
		StopAll();
		for (auto it = m_Players.begin(); it != m_Players.end(); it++)
		{
			(*it)->Reset();
		}
		m_TimeEclipsed = 0.0f;
	}

	std::shared_ptr<Player> PlayerManager::AddNewPlayer()
	{
		auto player = std::make_shared<Player>();
		m_Players.push_back(player);
		return player;
	}

	void PlayerManager::Delete(UUID id)
	{
		for (auto it = m_Players.begin(); it != m_Players.end(); it++)
		{
			if ((*it)->GetID() == id)
			{
				(*it)->Stop();
				(*it)->ClearTrack();
				m_Players.erase(it);
				return;
			}
		}
	}

	// Mostly deprecated, most use cases should just use GetTime(), GetTimeSeconds(), or GetTimeMs() instead
	unsigned int PlayerManager::GetPosition()
	{
		if (m_Players.size() == 0)
			return 0;

		for (auto it = m_Players.begin(); it != m_Players.end(); it++)
		{
			if ((*it)->IsPlaying())
				return (*it)->GetPosition();
		}

		return m_Players[0]->GetPosition();
	}

	TimeMinSec PlayerManager::GetTime()
	{
		TimeMinSec time = {
			.minutes      = 0,
			.seconds      = 0,
			.milliseconds = 0
		};

		if (m_Players.size() == 0)
			return time;

		if (m_Playing)
			return Audio::GetTime(m_Timer.GetTime() + m_TimeEclipsed);
		else
			return Audio::GetTime(m_TimeEclipsed);
	}

	TimeMinSec PlayerManager::GetTimeSeconds()
	{
		TimeMinSec time = {
			.minutes = 0,
			.seconds = 0,
			.milliseconds = 0
		};

		if (m_Players.size() == 0)
			return time;

		if (m_Playing)
			time.seconds = unsigned int(m_Timer.GetTime() + m_TimeEclipsed);
		else
			time.seconds = unsigned int(m_TimeEclipsed);

		return time;
	}

	TimeMinSec PlayerManager::GetTimeMs()
	{
		TimeMinSec time = {
			.minutes = 0,
			.seconds = 0,
			.milliseconds = 0
		};

		if (m_Players.size() == 0)
			return time;

		if (m_Playing)
			return Audio::GetTimeMs(m_Timer.GetTime() + m_TimeEclipsed);
		else
			return Audio::GetTimeMs(m_TimeEclipsed);
	}

	// Change to use seconds
	void PlayerManager::SetPosition(float seconds)
	{
		unsigned int position = 0;
		float byteRate = 0.0f;
		std::shared_ptr<Player> player = nullptr;

		for (auto it = m_Players.begin(); it != m_Players.end(); it++)
		{
			player = *it;
			byteRate = (float)player->GetTrack()->GetProps().byteRate;
			position = unsigned int(seconds * byteRate);
			player->SetPosition(position);

			if (m_Playing && !player->IsPlaying())
				player->Play();
		}
		m_TimeEclipsed = seconds;
		m_Timer.Reset();
	}

	std::shared_ptr<Player> PlayerManager::Find(UUID id)
	{
		for (auto it = m_Players.begin(); it != m_Players.end(); it++)
		{
			if ((*it)->GetID() == id)
				return *it;
		}
		return nullptr;
	}

}