#include "bbpch.h"

// TODO: Refactor code to work with AudioInfo struct insted of AudioData class

#include "BackBeat/Audio/Helpers/int24.h"
#include "Track.h"
namespace BackBeat {

	Track::Track(AudioInfo info)
	{
		m_Done = false;
		if (info.type == wav) {
			m_Position = info.dataZero;
		}
		else {
			m_Position = 0; // NOTE: Will change with implementation if needed with other file types
		}
		m_Volume = 1.0f;
		m_Info = info;
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
		unsigned int trueSize = m_Info.dataSize + m_Info.dataZero;
		if (numBytes + m_Position > trueSize)
			numBytes = trueSize - m_Position;

		std::ifstream file;
		file.open(m_Info.filePath, std::ios::binary);
		file.seekg(m_Position);

		if (Audio::IsBigEndian() == m_Info.props.bigEndian)
			file.read((char*)output, numBytes);
		else
			return false; // TODO: Implement way to switch endianness of track data
		
		MultiplyVolume(output, numBytes);

		m_Position += numBytes;
		if (m_Position >= trueSize)
			m_Done = true;
		file.close();
		return true;
	}

	TimeMinSec Track::GetTime()
	{
		TimeMinSec time = TimeMinSec();
		AudioProps props = m_Info.props;
		float timeTotal = (float)((m_Position - m_Info.dataZero) / props.byteRate);
		unsigned int minutes = (unsigned int)floor(timeTotal / 60);
		unsigned int seconds = (unsigned int)((unsigned int)timeTotal % 60);
		time.minutes = minutes;
		time.seconds = seconds;
		return time;
	}

	TimeMinSec Track::GetLength()
	{
		TimeMinSec time = TimeMinSec();
		AudioProps props = m_Info.props;
		float timeTotal = (float)(m_Info.dataSize / props.byteRate);
		unsigned int minutes = (unsigned int)floor(timeTotal / 60);
		unsigned int seconds = (unsigned int)((unsigned int)timeTotal % 60);
		time.minutes = minutes;
		time.seconds = seconds;
		return time;
	}

	void Track::SetPosition(unsigned int position)
	{
		unsigned int offset = position % m_Info.props.blockAlign;
		m_Position = position - offset + m_Info.dataZero;
		if (m_Position >= m_Info.dataSize) {
			m_Position = m_Info.dataSize + m_Info.dataZero;
			m_Done = true;
			return;
		}
		m_Done = false;
	}

	float Track::GetProgress()
	{
		float progress = ((float)m_Position - (float)m_Info.dataZero) / (float)m_Info.dataSize;
		return progress;
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