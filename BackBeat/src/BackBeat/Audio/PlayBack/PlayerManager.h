#pragma once

// New Multitrack Player class/system
// NEEDS:
// 1. Ability to play tracks of all bitdepths and samples
// 2. Ability to sync tracks to play at 0 at the same time or at least reset them all at the same time
// 3. Compatable with current mixer - processor relationship but not necessarily the current Player class and PlayerProc class
// 4. Compatable with current Track class
// 5. Able to display info from all Tracks
// 6. Able to add / delete tracks as desired (probably need to add the ability for Mixer to delete procs)
// 7. Able to play Recordings
// 8. Able to control all classes individually and insync
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