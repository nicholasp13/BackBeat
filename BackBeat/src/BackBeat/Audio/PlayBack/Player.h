#pragma once

// TODO: Create upsamplers and downsamplers for support of other sample rates

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
		void Stop();
		void LoadTrack(std::string filePath);
		
		TimeMinSec GetTime();
		TimeMinSec GetLength();
		unsigned int GetPosition();
		unsigned int GetSize();
		std::string GetTrackName();
		void SetPosition(unsigned int pos);
		void SetVolume(float vol);

		void Play() { m_PlayerProcessor->On(); }
		void Pause() { m_PlayerProcessor->Off(); }
		bool IsLoaded() { return m_SelectedTrack != nullptr; }
		bool IsPlaying() { return m_PlayerProcessor->IsOn(); }
		std::shared_ptr<PlayerProcessor> GetProc() { return m_PlayerProcessor; }

	private:
		Track* m_SelectedTrack;
		std::shared_ptr<PlayerProcessor> m_PlayerProcessor;

	};
}