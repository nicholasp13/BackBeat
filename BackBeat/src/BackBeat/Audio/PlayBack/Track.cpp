#include "bbpch.h"

#include "BackBeat/Audio/Helpers/int24.h"
#include "Track.h"
namespace BackBeat {

	Track::Track(AudioInfo info)
		:
		m_Done(false),
		m_Position(info.dataZero),
		m_StartPos(info.dataZero),
		m_EndPos(info.dataSize + info.dataZero),
		m_Volume(1.0f),
		m_Info(info)
	{

	}

	Track::~Track()
	{
		
	}

	// TODO: Implement endianness conversion if the track does not match the systems endianness
	bool Track::Render(byte* output, unsigned int numBytes) 
	{
		if (numBytes == 0)
			return true;
		if ((numBytes % m_Info.props.blockAlign) != 0)
			return false;

		unsigned int position = m_Position + numBytes;
		unsigned int bytesToRender = numBytes;
		if (m_Position < m_StartPos) 
		{
			if (position >= m_StartPos) 
			{
				bytesToRender = position - m_StartPos;
				m_Position = m_StartPos;
			}
			else 
			{
				m_Position = position;
				return true;
			}
		}
		else if (m_Position >= m_EndPos) 
		{
			m_Position += bytesToRender;
			if (m_Position >= m_Info.dataSize + m_Info.dataZero) 
			{
				m_Position = m_Info.dataSize + m_Info.dataZero;
				m_Done = true;
			}
			return true;
		}
		else if (position > m_EndPos)
			bytesToRender = m_EndPos - m_Position;

		std::ifstream file;
		file.open(m_Info.filePath, std::ios::binary);
		file.seekg(m_Position);

		if (Audio::IsBigEndian() == m_Info.props.bigEndian)
			file.read((char*)output, bytesToRender);
		else
			return false; // TODO: Implement way to switch endianness of track data
		
		MultiplyVolume(output, bytesToRender);

		m_Position += bytesToRender;
		if (m_Position >= m_Info.dataSize + m_Info.dataZero)
			m_Done = true;

		file.close();
		return true;
	}

	TimeMinSec Track::GetTime()
	{
		TimeMinSec time = TimeMinSec();
		AudioProps props = m_Info.props;
		float timeTotal = (float)((m_Position - m_Info.dataZero) / props.byteRate);
		return Audio::GetTime(timeTotal);
	}

	TimeMinSec Track::GetLength()
	{
		TimeMinSec time = TimeMinSec();
		AudioProps props = m_Info.props;
		float timeTotal = (float)(m_Info.dataSize / props.byteRate);
		return Audio::GetTime(timeTotal);
	}

	// NOTE: Not used and may be deleted (only used for testing but is currently uneeded)
	float Track::GetProgress()
	{
		float progress = ((float)m_Position - (float)m_Info.dataZero) / (float)m_Info.dataSize;
		return progress;
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
		m_StartPos = start - offset + m_Info.dataZero;
		if (m_StartPos >= m_Info.dataSize + m_Info.dataZero) 
		{
			m_StartPos = m_Info.dataSize + m_Info.dataZero;
			m_Done = true;
			return;
		}
		else if (m_StartPos <= m_Info.dataZero) 
		{
			m_StartPos = m_Info.dataZero;
		}
	}

	void Track::SetEnd(unsigned int end)
	{
		unsigned int offset = end % m_Info.props.blockAlign;
		m_EndPos = end - offset + m_Info.dataZero;
		if (m_EndPos >= m_Info.dataSize) 
		{
			m_EndPos = m_Info.dataSize + m_Info.dataZero;
		}
		else if (m_EndPos <= m_Info.dataZero) 
		{
			m_EndPos = m_Info.dataZero;
		}
	}

	void Track::MultiplyVolume(byte* output, unsigned int numBytes)
	{
		AudioProps props = m_Info.props;
		unsigned int numSamples = numBytes / props.blockAlign * props.numChannels;
		
		switch (props.bitDepth)
		{

		case (BYTE_BIT_SIZE):
		{
			for (unsigned int i = 0; i < numSamples; i++) {
				output[i] = (byte)((float)(output[i]) * m_Volume);
			}
			break;
		}

		case (INT16_BIT_SIZE):
		{
			auto buffer = reinterpret_cast<signed short*>(output);
			for (unsigned int i = 0; i < numSamples; i++) {
				buffer[i] = (signed short)((float)(buffer[i]) * m_Volume);
			}
			break;
		}

		case (INT24_BIT_SIZE):
		{
			auto buffer = reinterpret_cast<byte*>(output);
			int24 src = int24();
			byte srcBytes[INT24_BYTE_SIZE];
			for (unsigned int i = 0; i < numBytes; i+= INT24_BYTE_SIZE) {
				for (unsigned int j = 0; j < INT24_BYTE_SIZE; j++) {
					srcBytes[2 - j] = buffer[i + j];
				}
				src = int24(srcBytes[0], srcBytes[1], srcBytes[2]);
				src = int24(src.ToFloat() * m_Volume);
				output[i] = src[2];
				output[i + 1] = src [1];
				output[i + 2] = src[0];
			}
			break;
		}

		case (FLOAT_BIT_SIZE):
		{
			auto buffer = reinterpret_cast<float*>(output);
			for (unsigned int i = 0; i < numSamples; i++) {
				buffer[i] = buffer[i] * m_Volume;
			}
			break;
		}

		case (DOUBLE_BIT_SIZE):
		{
			auto buffer = reinterpret_cast<double*>(output);
			for (unsigned int i = 0; i < numSamples; i++) {
				buffer[i] = (double)(buffer[i] * m_Volume);
			}
			break;
		}

		default:
		{
			return;
		}

		}
	}

}