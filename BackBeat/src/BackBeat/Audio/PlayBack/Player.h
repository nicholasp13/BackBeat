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
		unsigned int GetByteRate();
		unsigned int GetPosition(); 
		unsigned int GetSampleRate();
		unsigned int GetSize();
		unsigned int GetStartPosition();
		unsigned int GetEndPosition();
		Track* GetTrack();
		std::string GetTrackName();
		void SetPosition(unsigned int pos);
		void SetStart(unsigned int start);
		void SetEnd(unsigned int end);
		void SetVolume(float vol);

		inline void Play() { m_PlayerProcessor->On(); }
		inline void Pause() { m_PlayerProcessor->Off(); }
		inline bool IsLoaded() { return m_SelectedTrack != nullptr; }
		inline bool IsPlaying() { return m_PlayerProcessor->IsOn(); }
		inline std::shared_ptr<PlayerProcessor> GetProc() { return m_PlayerProcessor; }

	private:
		Track* m_SelectedTrack;
		std::shared_ptr<PlayerProcessor> m_PlayerProcessor;

	};
}