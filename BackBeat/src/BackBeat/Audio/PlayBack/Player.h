#pragma once

// TODO: Add support for int24 data types
//       Create upsamplers and downsamplers for support of other sample rates

#include "TrackFactory.h"
#include "PlayerProcessor.h"
namespace BackBeat {

	// NOTE: Tracklist / multiple tracks will be implemented later as needed

	class Player
	{
	public:
		Player();
		~Player();

		void Start();
		void Pause();
		void Stop();
		void LoadTrack(std::string filePath);
		
		TimeMinSec GetTime();
		TimeMinSec GetLength();
		float GetProgress();
		void SetPosition(unsigned int pos);
		void SetVolume(float vol);

		bool IsLoaded() { return m_SelectedTrack != nullptr; }
		bool IsPlaying() { return m_PlayerProcessor->IsOn(); }
		std::string GetTrackName();
		std::shared_ptr<PlayerProcessor> GetProc() { return m_PlayerProcessor; }

	private:
		TrackFactory m_TrackFactory;
		Track* m_SelectedTrack;
		std::shared_ptr<PlayerProcessor> m_PlayerProcessor;

	};
}