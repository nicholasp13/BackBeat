#pragma once

namespace BackBeat {

	class AudioBuffer
	{
	public:
		AudioBuffer(tWAVEFORMATEX props);
		~AudioBuffer();
		
		tWAVEFORMATEX GetProperties() { return m_DeviceProps; }
		UINT32 GetSampleRate() { return m_SampleRate; }
		UINT32 GetSize() { return m_Size; }
		std::shared_ptr<float[]> GetBuffer() { return m_Buffer; }

	private:
		UINT32 m_Size;
		UINT32 m_SampleRate;
		std::shared_ptr<float[]> m_Buffer;
		tWAVEFORMATEX m_DeviceProps;
	};
}