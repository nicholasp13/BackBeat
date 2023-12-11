#include "bbpch.h"

#include "Track.h"
namespace BackBeat {

	Track::Track(AudioData* data)
	{
		m_Done = false;
		if (data->GetFileType() == FileType::WAV_FILE) {
			m_Position = data->GetZero();
		}
		else {
			m_Position = 0; // NOTE: Will change with implementation of mp3 files
		}
		m_Volume = 1.0f;
		m_Data = data;
	}

	Track::~Track()
	{
		delete m_Data;
	}

	// TODO: Implement endianness conversion if the track does not match the systems endianness
	bool Track::Render(byte* output, unsigned int numBytes) 
	{
		if (numBytes == 0)
			return true;
		if ((numBytes % m_Data->GetProps().blockAlign) != 0)
			return false;
		unsigned int trueSize = m_Data->GetDataSize() + m_Data->GetZero();
		if (numBytes + m_Position > trueSize)
			numBytes = trueSize - m_Position;

		std::ifstream file;
		file.open(m_Data->GetFilePath(), std::ios::binary);
		file.seekg(m_Position);

		if (Audio::IsBigEndian() == m_Data->GetProps().bigEndian)
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
		AudioProps props = m_Data->GetProps();
		float timeTotal = (float)((m_Position - m_Data->GetZero()) / props.byteRate);
		unsigned int minutes = (unsigned int)floor(timeTotal / 60);
		unsigned int seconds = (unsigned int)((unsigned int)timeTotal % 60);
		time.minutes = minutes;
		time.seconds = seconds;
		return time;
	}

	TimeMinSec Track::GetLength()
	{
		TimeMinSec time = TimeMinSec();
		AudioProps props = m_Data->GetProps();
		float timeTotal = (float)(m_Data->GetDataSize() / props.byteRate);
		unsigned int minutes = (unsigned int)floor(timeTotal / 60);
		unsigned int seconds = (unsigned int)((unsigned int)timeTotal % 60);
		time.minutes = minutes;
		time.seconds = seconds;
		return time;
	}

	void Track::SetPosition(unsigned int position)
	{
		if (position <= m_Data->GetZero()) {
			m_Position = m_Data->GetZero();
			m_Done = false;
			return;
		}
		unsigned int trueSize = m_Data->GetDataSize() + m_Data->GetZero();
		if (position >= trueSize) {
			return;
		}
		unsigned int offset = (position + m_Data->GetZero()) % m_Data->GetProps().blockAlign;
		m_Position = position - offset;
		if (m_Position < trueSize)
			m_Done = false;
		m_Done = true;
	}

	float Track::GetProgress()
	{
		float progress = ((float)m_Position - (float)m_Data->GetZero()) / (float)m_Data->GetDataSize();
		return progress;
	}

	void Track::MultiplyVolume(byte* output, unsigned int numBytes)
	{
		AudioProps props = m_Data->GetProps();
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
			// TODO: Implement INT24 data type
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