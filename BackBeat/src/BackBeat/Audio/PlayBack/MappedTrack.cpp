#include "bbpch.h"

#include "MappedTrack.h"
namespace BackBeat {

	MappedTrack::MappedTrack(AudioInfo info, std::shared_ptr<FileMap> fileMap)
		: 
		Track(info),
		m_Done(false),
		m_ReadPosition(info.dataZero),
		m_WritePosition(info.dataZero),
		m_StartPosition(info.dataZero),
		m_EndPosition(info.dataSize + info.dataZero),
		m_Volume(1.0f),
		m_Info(info), 
		m_FileMap(fileMap)
	{

	}

	MappedTrack::~MappedTrack()
	{
		if (m_Info.type == FileType::recordingTemp)
			m_FileMap->SetToDelete();
	}

	// NOT inherited from Track
	// Read used for visual representation of track. Does not set the track as done if goes passed the end position
	// Position needs to be on block align as well as numBytes
	bool MappedTrack::Read(byte* output, unsigned int numBytes, unsigned int position)
	{
		if (numBytes == 0)
			return true;
		if ((numBytes % m_Info.props.blockAlign) != 0)
			return false;
		if ((position % m_Info.props.blockAlign) != 0)
			return false;

		unsigned int newPosition = position + numBytes;
		unsigned int bytesToRender = numBytes;
		if (position < m_Info.dataZero)
		{
			if (newPosition >= m_Info.dataZero)
			{
				bytesToRender = newPosition - m_Info.dataZero;
				position = m_Info.dataZero;
			}
			else
			{
				return true;
			}
		}
		else if (position >= m_Info.dataSize + m_Info.dataZero)
		{
			return true;
		}
		else if (newPosition > m_Info.props.fileSize)
			bytesToRender = m_Info.props.fileSize - position;

		return m_FileMap->ReadData((char*)output, bytesToRender, position);
	}

	// NOT inherited from Track
	// Used for editing track, specifically overwriting specific parts of file.
	// Does not set the track as done if goes passed the end position
	// Position needs to be on block align as well as numBytes
	bool MappedTrack::Write(byte* input, unsigned int numBytes, unsigned int position)
	{
		if (numBytes == 0)
			return true;
		if ((numBytes % m_Info.props.blockAlign) != 0)
			return false;
		if ((position % m_Info.props.blockAlign) != 0)
			return false;

		unsigned int newPosition = position + numBytes;
		unsigned int bytesToRender = numBytes;
		if (position < m_Info.dataZero)
		{
			if (newPosition >= m_Info.dataZero)
			{
				bytesToRender = newPosition - m_Info.dataZero;
				position = m_Info.dataZero;
			}
			else
			{
				return true;
			}
		}
		else if (newPosition > m_Info.props.fileSize)
			bytesToRender = m_Info.props.fileSize - position;

		if (m_FileMap->WriteData((char*)input, bytesToRender, position))
		{
			if (newPosition >= m_Info.dataSize + m_Info.dataZero)
			{
				m_Info.dataSize = newPosition - m_Info.dataZero;
				m_EndPosition = newPosition;
			}

			return true;
		}

		return false;
	}

	void MappedTrack::SetReadPosition(unsigned int position)
	{
		unsigned int offset = position % m_Info.props.blockAlign;
		m_ReadPosition = position - offset + m_Info.dataZero;

		if (m_ReadPosition >= m_Info.dataSize + m_Info.dataZero)
		{
			m_ReadPosition = m_Info.dataSize + m_Info.dataZero;
			m_Done = true;
			return;
		}
		m_Done = false;
	}

	void MappedTrack::SetWritePosition(unsigned int position)
	{
		unsigned int offset = position % m_Info.props.blockAlign;
		m_WritePosition = position - offset + m_Info.dataZero;

		if (m_WritePosition >= m_Info.dataSize + m_Info.dataZero)
			m_WritePosition = m_Info.dataSize + m_Info.dataZero;
	}

	// Inherited from Track
	// Read function derived from Track class that is used by audio rendering, copying data, etc.
	bool MappedTrack::Read(byte* output, unsigned int numBytes)
	{
		if (numBytes == 0)
			return true;
		if ((numBytes % m_Info.props.blockAlign) != 0)
			return false;

		unsigned int position = m_ReadPosition + numBytes;
		unsigned int bytesToRender = numBytes;
		if (m_ReadPosition < m_StartPosition)
		{
			if (position >= m_StartPosition)
			{
				bytesToRender = position - m_StartPosition;
				m_ReadPosition = m_StartPosition;
			}
			else
			{
				m_ReadPosition = position;
				return true;
			}
		}
		else if (m_ReadPosition >= m_EndPosition)
		{
			m_ReadPosition += bytesToRender;
			if (m_ReadPosition >= m_Info.dataSize + m_Info.dataZero)
			{ 
				m_ReadPosition = m_Info.dataSize + m_Info.dataZero;
				m_Done = true;
			}
			return true;
		}
		else if (position > m_EndPosition)
			bytesToRender = m_EndPosition - m_ReadPosition;

		m_FileMap->ReadData((char*)output, bytesToRender, m_ReadPosition);

		// Multiply output by the volume
		Audio::MultiplyBufferByValue(output, bytesToRender, m_Info.props, m_Volume);

		m_ReadPosition += bytesToRender;
		if (m_ReadPosition >= m_Info.dataSize + m_Info.dataZero)
			m_Done = true;

		return true;
	}

	// Inherited from Track
	// While regular Track appends data to the end of the file, MappedTrack overwrites the MappedFile
	// Used for recording and copying data
	bool MappedTrack::Write(byte* input, unsigned int numBytes)
	{
		if (numBytes == 0)
			return true;
		if ((numBytes % m_Info.props.blockAlign) != 0)
			return false;

		if (input)
			m_FileMap->WriteData((char*)input, numBytes, m_WritePosition);
		else
		{
			// Write silence here
		}

		m_WritePosition += numBytes;
		if (m_WritePosition > m_Info.dataSize + m_Info.dataZero)
		{
			m_Info.dataSize = m_WritePosition - m_Info.dataZero;
			m_EndPosition = m_WritePosition;
		}
		return true;
	}

	void MappedTrack::Reset()
	{
		Clear();
		m_ReadPosition = m_Info.dataZero;
		m_WritePosition = m_Info.dataZero;
		m_StartPosition = m_Info.dataZero;
		m_EndPosition = m_Info.dataZero;
		m_Info.dataSize = 0;
	}

	void MappedTrack::Reset(AudioProps props)
	{
		m_Info.props = props;
		Reset();
	}

	void MappedTrack::Clear()
	{
		// Write 0s for DataSize in FileMap
		m_WritePosition = 0;
		auto size = GetSize();
		unsigned int count = 0;

		const unsigned int bufferSize = 4800;
		byte buffer[bufferSize] = {};
		Audio::FlushBuffer(buffer, bufferSize);

		while (count < size)
		{
			Write(buffer, bufferSize);
			count += bufferSize;
		}
	}

	TimeMinSec MappedTrack::GetTime()
	{
		AudioProps props = m_Info.props;
		float timeTotal = ((float)m_ReadPosition - (float)m_Info.dataZero) / (float)props.byteRate;
		return Audio::GetTime(timeTotal);
	}

	TimeMinSec MappedTrack::GetTimeMs()
	{
		AudioProps props = m_Info.props;
		float timeTotal = ((float)m_ReadPosition - (float)m_Info.dataZero) / (float)props.byteRate;
		return Audio::GetTimeMs(timeTotal);
	}

	TimeMinSec MappedTrack::GetLength()
	{
		AudioProps props = m_Info.props;
		float timeTotal = (float)m_Info.dataSize / (float)props.byteRate;
		return Audio::GetTime(timeTotal);
	}

	void MappedTrack::SetPosition(unsigned int position)
	{
		unsigned int offset = position % m_Info.props.blockAlign;
		m_ReadPosition = position - offset + m_Info.dataZero;

		if (m_ReadPosition >= m_Info.dataSize + m_Info.dataZero)
		{
			m_ReadPosition = m_Info.dataSize + m_Info.dataZero;
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