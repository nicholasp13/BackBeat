#include "bbpch.h"

#include "BackBeat/Audio/Helpers/AudioFileWriter.h"
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
		Delete();
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
		// TODO: Check if frames include the number of samples
		unsigned int numBytes = numFrames * m_Props.blockAlign;

		// If data is nullptr then silence is recordeded instead of samples from data
		if (data)
		{
			bool success = AudioFileWriter::WriteAudioFileData(m_TempPath, data, numBytes);
			if (success)
				m_Size += numBytes;
		}
		else
		{
			bool success = AudioFileWriter::WriteAudioFileSilence(m_TempPath, numBytes);
			if (success)
				m_Size += numBytes;
		}
	}

	void Recording::Reset()
	{
		Delete();
	}

	void Recording::Reset(AudioProps props)
	{
		Delete();
		m_Props = props;
	}

	// TODO: TEST IN WINDOWS RECORDER PLAYER
	// NOTES: 
	//  - Position is indexed by bytes not by samples or audio frames.
	//  - Function caller is tasked to make sure that numBytes is not greater than the buffer size
	void Recording::GetData(char* buffer, unsigned long numBytes, unsigned long position)
	{
		if (!buffer)
			return;
		if (position > m_Size)
			return;

		unsigned long actualNumBytes = position + numBytes < m_Size ? numBytes : m_Size - position;
		std::ifstream file;
		file.open(m_TempPath, std::ios::binary);
		if (!file.is_open())
			return;

		file.seekg(position);
		file.getline(buffer, actualNumBytes);
		
	}

	TimeMinSec Recording::GetLengthMinSecs()
	{
		float totalSeconds = (float)m_Size / (float)m_Props.byteRate;
		return Audio::GetTime(totalSeconds);
	}

	void Recording::Delete()
	{
		std::remove(m_TempPath.c_str());
		m_Size = 0;
	}

}