#include "bbpch.h"

#include "BackBeat/Audio/FileIO/AudioFileReader.h"
#include "BackBeat/Audio/FileIO/AudioFileWriter.h"
#include "Track.h"
namespace BackBeat {

	Track::Track(AudioInfo info)
		:
		m_Done(false),
		m_Position(info.dataZero),
		m_StartPosition(info.dataZero),
		m_EndPosition(info.dataSize + info.dataZero),
		m_Volume(1.0f),
		m_Info(info)
	{
		
	}

	Track::~Track()
	{

	}

	// Implement endianness conversion if the track does not match the systems endianness
	bool Track::Read(byte* output, unsigned int numBytes) 
	{
		if (numBytes == 0)
			return true;
		if ((numBytes % m_Info.props.blockAlign) != 0)
			return false;

		unsigned int position = m_Position + numBytes;
		unsigned int bytesToRender = numBytes;
		if (m_Position < m_StartPosition) 
		{
			if (position >= m_StartPosition) 
			{
				bytesToRender = position - m_StartPosition;
				m_Position = m_StartPosition;
			}
			else 
			{
				m_Position = position;
				return true;
			}
		}
		else if (m_Position >= m_EndPosition) 
		{
			m_Position += bytesToRender;
			if (m_Position >= m_Info.dataSize + m_Info.dataZero) 
			{
				m_Position = m_Info.dataSize + m_Info.dataZero;
				m_Done = true;
			}
			return true;
		}
		else if (position > m_EndPosition)
			bytesToRender = m_EndPosition - m_Position;

		std::ifstream file;
		file.open(m_Info.filePath, std::ios::binary);
		file.seekg(m_Position);

		if (Audio::IsBigEndian() == m_Info.props.bigEndian)
			file.read((char*)output, bytesToRender);
		else
			return false; // Implement way to switch endianness of track data
		
		// Multiply output by the volume
		Audio::MultiplyBufferByValue(output, bytesToRender, m_Info.props, m_Volume);

		m_Position += bytesToRender;
		if (m_Position >= m_Info.dataSize + m_Info.dataZero)
			m_Done = true;

		file.close();
		return true;
	}

	// Usually want file to be an empty file or maybe just its header before writing to it
	bool Track::Write(byte* input, unsigned int numBytes)
	{
		if (numBytes == 0)
			return true;
		if ((numBytes % m_Info.props.blockAlign) != 0)
			return false;

		unsigned int bytesToRender = numBytes;

		std::ofstream file;
		file.open(m_Info.filePath, std::ios::binary | std::ios::app);

		if (Audio::IsBigEndian() == m_Info.props.bigEndian)
			file.write((char*)input, bytesToRender);
		else
			return false; // Implement way to switch endianness of track data

		m_Info.dataSize += bytesToRender;
		m_EndPosition += bytesToRender;

		file.close();
		return true;
	}

	void Track::Reset()
	{
		std::remove(m_Info.filePath.c_str());
		m_Position = 0;
		m_StartPosition = 0;
		m_EndPosition = 0;
		m_Info.dataSize = 0;
	}

	void Track::Reset(AudioProps props)
	{
		m_Info.props = props;
		Reset();
	}

	void Track::Clear()
	{
		Reset();
	}

	TimeMinSec Track::GetTime()
	{
		AudioProps props = m_Info.props;
		float timeTotal = ((float)m_Position - (float)m_Info.dataZero) / (float)props.byteRate;
		return Audio::GetTime(timeTotal);
	}

	TimeMinSec Track::GetTimeMs()
	{
		AudioProps props = m_Info.props;
		float timeTotal = (((float)m_Position - (float)m_Info.dataZero) / (float)props.byteRate);
		return Audio::GetTimeMs(timeTotal);
	}

	TimeMinSec Track::GetLength()
	{
		AudioProps props = m_Info.props;
		float timeTotal = (float)m_Info.dataSize / (float)props.byteRate;
		return Audio::GetTime(timeTotal);
	}

	void Track::SetPosition(unsigned int position)
	{
		unsigned int offset = position % m_Info.props.blockAlign;
		m_Position = position - offset + m_Info.dataZero;
		if (m_Position >= m_Info.dataSize + m_Info.dataZero) 
		{
			m_Position = m_Info.dataSize + m_Info.dataZero;
			m_Done = true;
			return;
		}
		m_Done = false;
	}

	void Track::SetStart(unsigned int start)
	{
		unsigned int offset = start % m_Info.props.blockAlign;
		m_StartPosition = start - offset + m_Info.dataZero;
		if (m_StartPosition >= m_Info.dataSize + m_Info.dataZero) 
		{
			m_StartPosition = m_Info.dataSize + m_Info.dataZero;
			m_Done = true;
			return;
		}
		else if (m_StartPosition <= m_Info.dataZero) 
		{
			m_StartPosition = m_Info.dataZero;
		}
	}

	void Track::SetEnd(unsigned int end)
	{
		unsigned int offset = end % m_Info.props.blockAlign;
		m_EndPosition = end - offset + m_Info.dataZero;
		if (m_EndPosition >= m_Info.dataSize) 
		{
			m_EndPosition = m_Info.dataSize + m_Info.dataZero;
		}
		else if (m_EndPosition <= m_Info.dataZero) 
		{
			m_EndPosition = m_Info.dataZero;
		}
	}

}