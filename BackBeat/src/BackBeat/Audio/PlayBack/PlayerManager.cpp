#include "bbpch.h"

// TODO: Set and create timer

#include "PlayerManager.h"
namespace BackBeat {

	PlayerManager::PlayerManager()
		: m_Playing(false)
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
	}

	void PlayerManager::StopAll()
	{
		for (auto it = m_Players.begin(); it != m_Players.end(); it++)
		{
			(*it)->Stop();
		}
		m_Playing = false;
	}

	void PlayerManager::PlayAll()
	{
		for (auto it = m_Players.begin(); it != m_Players.end(); it++)
		{
			(*it)->Play();
		}
		m_Playing = true;
	}

	void PlayerManager::PauseAll()
	{
		for (auto it = m_Players.begin(); it != m_Players.end(); it++)
		{
			(*it)->Pause();
		}
		m_Playing = false;
	}

	void PlayerManager::ResetAll()
	{
		StopAll();
		for (auto it = m_Players.begin(); it != m_Players.end(); it++)
		{
			(*it)->Reset();
		}
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

	// Assumes all positions for all active players should be the same
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

		for (auto it = m_Players.begin(); it != m_Players.end(); it++)
		{
			if ((*it)->IsPlaying())
			{
				return (*it)->GetTime();
			}
		}

		return m_Players[0]->GetTime();
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

		for (auto it = m_Players.begin(); it != m_Players.end(); it++)
		{
			if ((*it)->IsPlaying())
			{
				return (*it)->GetTimeMs();
			}
		}

		return m_Players[0]->GetTimeMs();
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
		}
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