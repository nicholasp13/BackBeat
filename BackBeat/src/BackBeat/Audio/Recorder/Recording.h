#pragma once

#include "BackBeat/Audio/Audio.h" 
#include "BackBeat/Audio/FileIO/AudioFileLoader.h"
#include "BackBeat/Audio/PlayBack/MappedTrack.h"
namespace BackBeat {

	class Recording
	{
	public:
		Recording(std::string filePath, AudioProps props);
		Recording(std::shared_ptr<MappedTrack> track);
		~Recording();

		bool SaveWAV(std::string filePath);
		void Record(char* data, unsigned int numFrames);
		void Record(char* data, unsigned int numFrames, AudioProps props);
		void Reset();
		// NOTE: This Reset() would likely need to reformat data as most of the time users would not want
		//       all their data to be wiped just because they changes props/settings.
		void Reset(AudioProps props);
		void ClearTrack();
		void SetTrack(std::shared_ptr<MappedTrack> track);
		TimeMinSec GetLengthMinSecs();
		void SetIndex(unsigned int index);
		
		inline AudioProps GetProps() { return m_Props; }
		inline unsigned long GetSize() { return m_Size; }
		inline std::shared_ptr<MappedTrack> GetTrack() { return m_Track; }

	private:
		unsigned int m_Index;
		unsigned long m_Size;
		std::string m_TempPath;

		AudioProps m_Props;
		AudioFileLoader m_Loader;
		std::shared_ptr<MappedTrack> m_Track;

	};

}