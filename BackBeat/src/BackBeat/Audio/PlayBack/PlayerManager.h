#pragma once

#include "BackBeat/Audio/Recorder/Recording.h"
#include "BackBeat/Core/Timer.h"
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

		unsigned int GetPosition();
		TimeMinSec GetTime();
		TimeMinSec GetTimeSeconds();
		TimeMinSec GetTimeMs();
		void SetPosition(float seconds);

		inline bool IsPlaying() { return m_Playing; }
		inline unsigned long GetFileLimit() { return s_FileLimitSize; }
		inline std::shared_ptr<Player> GetPlayer(UUID id) { return Find(id); }
		
	private:
		static const unsigned long s_FileLimitSize = 230400000ul; // 10 mins in bytes at the byteRate of 384k
		                                                          // (48k sample rate, 32bit floating point, stereo)

		bool m_Playing;
		float m_TimeEclipsed;
		BackBeat::Timer m_Timer;
		std::vector< std::shared_ptr<Player> > m_Players;

	private:
		std::shared_ptr<Player> Find(UUID id);

	};

}