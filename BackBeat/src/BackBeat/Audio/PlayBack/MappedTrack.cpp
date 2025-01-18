#include "bbpch.h"

#include "MappedTrack.h"
namespace BackBeat {

	MappedTrack::MappedTrack(AudioInfo info, std::shared_ptr<FileMap> fileMap)
		: 
		Track(info),
		m_Done(false),
		m_Position(info.dataZero),
		m_StartPosition(info.dataZero),
		m_EndPosition(info.dataSize + info.dataZero),
		m_Volume(1.0f),
		m_Info(info), 
		m_FileMap(fileMap)
	{

	}

	MappedTrack::~MappedTrack()
	{

	}

	bool MappedTrack::Read(byte* output, unsigned int numBytes)
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

		m_FileMap->ReadData((char*)output, bytesToRender, m_Position);

		// Multiply output by the volume
		Audio::MultiplyBufferByValue(output, bytesToRender, m_Info.props, m_Volume);

		m_Position += bytesToRender;
		if (m_Position >= m_Info.dataSize + m_Info.dataZero)
			m_Done = true;

		return true;
	}

	// While regular Track appends data to the end of the file, MappedTrack overwrites the MappedFile
	bool MappedTrack::Write(byte* input, unsigned int numBytes)
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

		m_FileMap->WriteData((char*)input, bytesToRender, m_Position);

		m_Position += bytesToRender;
		if (m_Position >= m_Info.dataSize + m_Info.dataZero)
			m_Done = true;

		return true;
	}

	TimeMinSec MappedTrack::GetTime()
	{
		AudioProps props = m_Info.props;
		float timeTotal = (float)((m_Position - m_Info.dataZero) / props.byteRate);
		return Audio::GetTime(timeTotal);
	}

	TimeMinSec MappedTrack::GetLength()
	{
		AudioProps props = m_Info.props;
		float timeTotal = (float)(m_Info.dataSize / props.byteRate);
		return Audio::GetTime(timeTotal);
	}

	void MappedTrack::SetPosition(unsigned int position)
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

	void MappedTrack::SetStart(unsigned int start)
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

	void MappedTrack::SetEnd(unsigned int end)
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