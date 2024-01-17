#include "bbpch.h"

#include "Sample.h"
namespace BackBeat {

	Sample::Sample(AudioInfo info)
		: m_Done(false), m_Position(info.dataZero), m_Info(info)
	{

	}

	Sample::~Sample()
	{

	}

	bool Sample::Render(byte* output, unsigned int numBytes)
	{
		if (numBytes == 0)
			return true;
		if ((numBytes % m_Info.props.blockAlign) != 0)
			return false;

		const unsigned int size = m_Info.dataSize + m_Info.dataZero;
		unsigned int bytesToRender = (m_Position + numBytes) > size ? (size - m_Position) : numBytes;

		std::ifstream file;
		file.open(m_Info.filePath, std::ios::binary);
		file.seekg(m_Position);
		if (Audio::IsBigEndian() == m_Info.props.bigEndian)
			file.read((char*)output, bytesToRender);
		else
			return false; // TODO: Implement way to switch endianness of data

		m_Position += bytesToRender;
		if (m_Position >= size)
			Off();

		return true;
	}

	void Sample::Off()
	{
		m_Position = m_Info.dataZero;
		m_Done = true;
	}

	void Sample::Reset()
	{
		m_Position = m_Info.dataZero;
		m_Done = false;
	}
}