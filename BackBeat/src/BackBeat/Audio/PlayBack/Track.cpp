#include "bbpch.h"

#include "BackBeat/Audio/Helpers/int24.h"
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
		// NOTE: Usually the Recording class would delete its temp file during its destructrion
		//       but this is a safeguard just in case its Track changes
		if (m_Info.type == FileType::recordingTemp)
			std::remove(m_Info.filePath.c_str());
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
			return false; // TODO: Implement way to switch endianness of track data
		
		MultiplyVolume(output, bytesToRender);

		m_Position += bytesToRender;
		if (m_Position >= m_Info.dataSize + m_Info.dataZero)
			m_Done = true;

		file.close();
		return true;
	}

	// Rewrites/replaces current data data. May want to move this somewhere else for better SRP. Fine for now.
	bool Track::CopyData(AudioInfo srcInfo)
	{
		if (m_Info.props != srcInfo.props)
			return false;
		if (m_Info.type != FileType::recordingTemp)
			return false;
			
		std::remove(m_Info.filePath.c_str());

		const unsigned int arraySize = 5000;
		char data[arraySize] = {};
		unsigned int size = srcInfo.dataSize + srcInfo.dataZero;
		const unsigned int dataSize = arraySize;
		const unsigned int fileSize = size;
		unsigned int filePosition = srcInfo.dataZero;
		unsigned int dataIncrement = dataSize;
		 
		bool success = true;
		while (filePosition < fileSize && success)
		{
			dataIncrement = (dataSize + filePosition) <= fileSize ? dataSize : (fileSize - filePosition);
			success = AudioFileReader::ReadAudioFileData(srcInfo.filePath, data, filePosition, dataIncrement);
			success = AudioFileWriter::WriteAudioFileData(m_Info.filePath, data, dataIncrement);
			filePosition += dataIncrement;
		}

		if (success)
		{
			m_Info.dataSize = srcInfo.dataSize;
			m_Info.props.fileSize = srcInfo.props.fileSize;
			m_StartPosition = 0;
			m_Position = 0;
			m_EndPosition = srcInfo.dataSize;
		}

		return success;
	}

	TimeMinSec Track::GetTime()
	{
		AudioProps props = m_Info.props;
		float timeTotal = (float)((m_Position - m_Info.dataZero) / props.byteRate);
		return Audio::GetTime(timeTotal);
	}

	TimeMinSec Track::GetLength()
	{
		AudioProps props = m_Info.props;
		float timeTotal = (float)(m_Info.dataSize / props.byteRate);
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

	void Track::MultiplyVolume(byte* output, unsigned int numBytes)
	{
		AudioProps props = m_Info.props;
		unsigned int numSamples = numBytes / props.blockAlign * props.numChannels;
		
		switch (props.bitDepth)
		{

		case (Audio::ByteBitSize):
		{
			for (unsigned int i = 0; i < numSamples; i++) {
				output[i] = (byte)((float)(output[i]) * m_Volume);
			}
			break;
		}

		case (Audio::Int16BitSize):
		{
			auto buffer = reinterpret_cast<signed short*>(output);
			for (unsigned int i = 0; i < numSamples; i++) {
				buffer[i] = (signed short)((float)(buffer[i]) * m_Volume);
			}
			break;
		}

		case (Audio::Int24BitSize):
		{
			int24* intBuffer = int24::GetInt24Buffer(output, numSamples, m_Info.props.bigEndian);
			for (unsigned int i = 0; i < numSamples; i++) {
				intBuffer[i] = int24((float)intBuffer[i] * m_Volume);
			}

			byte* byteBuffer = int24::GetByteBuffer(intBuffer, numSamples, m_Info.props.bigEndian);
			Audio::CopyInputToOutput(output, byteBuffer, numSamples * Audio::Int24ByteSize);
			delete[] intBuffer;
			delete[] byteBuffer;
			break;
		}

		case (Audio::FloatBitSize):
		{
			auto buffer = reinterpret_cast<float*>(output);
			for (unsigned int i = 0; i < numSamples; i++) {
				buffer[i] = buffer[i] * m_Volume;
			}
			break;
		}

		case (Audio::DoubleBitSize):
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