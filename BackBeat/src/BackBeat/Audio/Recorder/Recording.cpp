#include "bbpch.h"

#include "BackBeat/Audio/FileIO/AudioFileWriter.h"
#include "Recording.h"
namespace BackBeat {

	Recording::Recording(std::string filePath, AudioProps props)
		:
		m_Size(0),
		m_TempPath(filePath), 
		m_Props(props)
	{
		
	}

	Recording::~Recording()
	{
		
	}

	bool Recording::SaveWAV(std::string filePath)
	{
		bool success = false;
		unsigned long dataSize = m_Size;

		// Write WAV header
		success = AudioFileWriter::WriteWAVFileHeader(filePath, m_Props, dataSize);
		if (!success)
		{
			std::remove(filePath.c_str());
			return success;
		}

		// NOTE: Might want to refactor this into a static helper function if needed in other classes
		// Copies data from temp file (srcFile) to the new file specificed by filePath (targetFile)
		char data = (char)0;
		std::ifstream srcFile;
		std::ofstream targetFile;

		srcFile.open(m_TempPath, std::ios::binary);
		if (!srcFile.is_open())
		{
			std::remove(filePath.c_str());
			return false;
		}

		targetFile.open(filePath.c_str(), std::ios::binary | std::ios::app);
		if (!targetFile.is_open())
		{
			std::remove(filePath.c_str());
			return false;
		}

		// TODO: Fix this (works its just stupid/slow)
		for (unsigned long i = 0; i < dataSize; i++)
		{
			srcFile.seekg(i);
			srcFile.get(data);
			targetFile.put(data);
		}

		srcFile.close();
		targetFile.close();

		return success;
	}

	// numFrames = numSamples / numChannels
	void Recording::Record(char* data, unsigned int numFrames)
	{
		if (!m_Track)
			return;

		unsigned int numBytes = numFrames * m_Props.blockAlign;

		// If data is nullptr then silence is recordeded instead of samples from data
		if (data)
		{
			if (m_Track->Write((byte*)data, numBytes))
				m_Size += numBytes; 
		}
		else
		{
			// TODO: Write silence to Track 
			//       (this only happens in WindowsRenderer where sometimes the function that returns the buffer of
			//       data returns nullptr to indicate silence. However this has never happened in testing so it is 
			//       fine for now for this to do nothing)
		}

	}

	void Recording::Reset()
	{
		m_Track->Reset();
	}

	void Recording::Reset(AudioProps props)
	{
		m_Props = props;
		if (m_Track)
			m_Track->Reset(props);
	}

	void Recording::ClearTrack()
	{
		m_Track = nullptr;
		m_TempPath = "";
		m_Size = 0;
	}

	void Recording::SetTrack(std::shared_ptr<Track> track)
	{
		if (!track)
			return;

		m_Size = track->GetSize();
		m_TempPath = track->GetInfo().filePath;
		m_Track = track;
	}

	TimeMinSec Recording::GetLengthMinSecs()
	{
		float totalSeconds = (float)m_Size / (float)m_Props.byteRate;
		return Audio::GetTime(totalSeconds);
	}

}