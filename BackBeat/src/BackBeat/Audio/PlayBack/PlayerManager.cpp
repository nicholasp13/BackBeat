#include "bbpch.h"

// NOTE: This is only partially implemented as the complete version will check the size and/or state of each player
//       and update accordingly. Currently this class does not pause or stop by itself when all the players are done playing

#include "PlayerManager.h"
namespace BackBeat {

	PlayerManager::PlayerManager()
		: m_Playing(false), m_Position(0u)
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