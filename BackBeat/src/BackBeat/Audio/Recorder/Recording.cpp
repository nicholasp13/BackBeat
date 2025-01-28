#include "bbpch.h"

#include "BackBeat/Audio/FileIO/AudioFileWriter.h"
#include "Recording.h"
namespace BackBeat {

	Recording::Recording(std::string filePath, AudioProps props)
		:
		m_Index(0),
		m_Size(0),
		m_TempPath(filePath), 
		m_Props(props),
		m_Loader(props.byteRate),
		m_Track(nullptr)
	{
		
	}

	Recording::Recording(std::shared_ptr<MappedTrack> track)
		:
		m_Index(0),
		m_Size(track->GetSize()),
		m_TempPath(track->GetFilePath()),
		m_Props(track->GetProps()),
		m_Loader(track->GetProps().byteRate),
		m_Track(track)
	{

	}

	Recording::~Recording()
	{
		
	}

	// TODO: FIX
	// Mostly deprecated, most use cases should just use TrackFactory::CopyTrack()
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

	// Used when input props and output (this) props are the same i.e. recording BackBeat instruments
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


	// Used for when input props can be different than output (this) props i.e. getting a single recording channel from a 
	// multitrack recording buffer from a Windows device
	// @params 
	//     - data         = input data
	//     - numFrames    = numSamples / numChannels
	//     - props        = input data props
	//     - channelIndex = the index of which channel to record from if props.numChannels = mono
	void Recording::Record(char* data, unsigned int numFrames, AudioProps props)
	{
		if (!m_Track)
			return;

		unsigned int totalInputBytes = numFrames * props.blockAlign;

		// If data is nullptr then silence is recordeded instead of samples from data
		if (!data)
		{
			// TODO: Write silence to Track 
			//       (this only happens in WindowsRenderer where sometimes the function that returns the buffer of
			//       data returns nullptr to indicate silence. However this has never happened in testing so it is 
			//       fine for now for this to do nothing)
		}
		else if (m_Props == props)
		{
			if (m_Track->Write((byte*)data, totalInputBytes))
				m_Size += totalInputBytes;
		}
		else
		{
			if (m_Loader.Unload(m_Track, (byte*)data, props, totalInputBytes, m_Index))
				m_Size += totalInputBytes;
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
		m_Track->Clear();
		m_TempPath = "";
		m_Size = 0;
	}

	void Recording::SetTrack(std::shared_ptr<MappedTrack> track)
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

	void Recording::SetIndex(unsigned int index)
	{
		if (index > m_Props.numChannels)
			return;

		m_Index = index;
	}

}