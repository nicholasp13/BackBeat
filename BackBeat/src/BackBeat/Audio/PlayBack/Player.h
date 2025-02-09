#pragma once

#include "TrackFactory.h"
#include "PlayerProcessor.h"
namespace BackBeat {

	class Player
	{
	public:
		Player();
		~Player();

		void Off();
		void Start();
		void Stop();
		void Play();
		void LoadTrack(std::string filePath);
		void LoadTrack(std::shared_ptr<Track> track);
		void ClearTrack();

		TimeMinSec GetTime();
		TimeMinSec GetTimeMs();
		TimeMinSec GetLength();
		unsigned int GetByteRate();
		unsigned int GetNumChannels();
		unsigned int GetPosition(); 
		unsigned int GetSampleRate();
		unsigned int GetSize();
		unsigned int GetStartPosition();
		unsigned int GetEndPosition();
		std::string GetTrackName();
		std::string GetTrackFilePath();
		void SetPosition(unsigned int pos);
		void SetStart(unsigned int start);
		void SetEnd(unsigned int end);
		void SetVolume(float vol);
		
		inline void On() { m_On = true; }
		inline void Pause() { m_PlayerProcessor->Off(); }
		inline void Reset() { Stop(); }
		inline bool IsOn() { return m_On; }
		inline bool IsLoaded() { return m_SelectedTrack != nullptr; }
		inline bool IsPlaying() { return m_PlayerProcessor->IsOn(); }
		inline std::shared_ptr<PlayerProcessor> GetProc() { return m_PlayerProcessor; }
		inline std::shared_ptr<Track> GetTrack() { return m_SelectedTrack; }
		inline UUID GetID() { return m_PlayerProcessor->GetID(); }

	private:
		bool m_On;
		std::shared_ptr<Track> m_SelectedTrack;
		std::shared_ptr<PlayerProcessor> m_PlayerProcessor;

	};
}