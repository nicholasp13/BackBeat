#pragma once

#include "BackBeat/Audio/Audio.h" 
#include "BackBeat/Audio/PlayBack/Track.h"
namespace BackBeat {

	class Recording
	{
	public:
		Recording(std::string filePath, AudioProps props);
		~Recording();

		bool SaveWAV(std::string filePath);
		void Record(char* data, unsigned int numFrames);
		void Reset();
		void Reset(AudioProps props);
		void CreateTrack();
		void ClearTrack();
		void SetTrack(std::shared_ptr<Track> track);
		TimeMinSec GetLengthMinSecs();
		
		inline AudioProps GetProps() { return m_Props; }
		inline unsigned long GetSize() { return m_Size; }
		inline std::shared_ptr<Track> GetTrack() { return m_Track; }

	private:
		unsigned long m_Size;
		std::string m_TempPath;
		AudioProps m_Props;
		std::shared_ptr<Track> m_Track;

	private:
		void Delete();

	};

}