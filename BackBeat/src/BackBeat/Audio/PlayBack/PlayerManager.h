#pragma once

#include "Track.h"
#include "BackBeat/Audio/Recorder/Recording.h"
#include "Player.h"
namespace BackBeat {

	class PlayerManager
	{
	public:
		PlayerManager();
		~PlayerManager();

		void PlayAll();
		void PauseAll();
		void StartAll();
		void StopAll();
		void ResetAll();
		std::shared_ptr<Player> AddNewPlayer();
		void Delete(UUID id);

		inline bool IsPlaying() { return m_Playing; }
		inline std::shared_ptr<Player> GetPlayer(UUID id) { return Find(id); }
		
	private:
		bool m_Playing;
		unsigned int m_Position;
		std::vector< std::shared_ptr<Player> > m_Players;

	private:
		std::shared_ptr<Player> Find(UUID id);

	};

}