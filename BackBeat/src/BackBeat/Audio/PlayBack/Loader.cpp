#include "bbpch.h"

#include "BackBeat/Audio/Audio.h"
#include "Loader.h"

/* README
* TODO:
* - Create checks to see if m_DataSrc was set before calls to Start() and GetData()
* - CREATE CONVERSION FOR DIFFERENT SAMPLE RATES
* DIFFERENT BYTE/NUMBER FORMATS MP3 FORMAT
*/
namespace BackBeat {

	Loader::Loader(tWAVEFORMATEX* deviceProps)
		: 
		m_DataSize(0),
		m_Data(nullptr),
		m_DataSrc(nullptr),
		m_DeviceProps(*deviceProps),
		m_BytePosition(WAV_HEADER_SIZE),
		m_SamplePosition(WAV_HEADER_SIZE)
	{
	}

	Loader::~Loader()
	{
	}

	void Loader::Start()
	{
		if (Loading) {
			BB_CORE_INFO("ALREADY LOADING");
			return;
		}
		if (Loaded) {
			BB_CORE_INFO("ALREADY LOADED");
			return;
		}

		Loading = true;
		BB_CORE_INFO("LOADING STARTED");
		Load();
	}

	void Loader::Stop()
	{
		Loading = false;
		BB_CORE_INFO("LOADING STOPPED");
	}

	HRESULT Loader::GetData(UINT32 framesAvailable, BYTE* buffer,
		UINT32* position, bool* playing)
	{
		UINT32 totalBytes = framesAvailable * m_DeviceProps.nBlockAlign;

		if (totalBytes + *position >= m_DataSize) 
			totalBytes = m_DataSize - *position;
		
		memcpy(buffer, m_Data + *position, totalBytes);
		*position += totalBytes;

		if (*position >= m_DataSize) {
			*playing = FALSE;
		}

		return S_OK;
	}

	HRESULT Loader::SetDataSource(AudioData* dataSource)
	{
		m_DataSrc = dataSource;
		m_Data = new BYTE[m_DataSize];
		m_DataSize = dataSource->GetSize() * m_DeviceProps.nBlockAlign / dataSource->GetProperties()->nBlockAlign;
		return S_OK;
	}

	void Loader::Load()
	{	
		if (Loaded) 
			return;

		HRESULT hr = S_OK;
		UINT32 bufferSize = m_DataSrc->GetProperties()->nAvgBytesPerSec;
		UINT32 sampleBufferSize = m_DataSrc->GetProperties()->nSamplesPerSec * m_DataSrc->GetProperties()->nChannels;
		BYTE* bufferData = new BYTE[bufferSize];

		while (Loading) {

			hr = m_DataSrc->LoadBuffer(bufferSize, bufferData, &m_BytePosition, &Loading);
			CHECK_FAILURE(hr);

			if (m_BytePosition >= m_DataSrc->GetSize()) {
				Loading = false;
				Loaded = true;
				BB_CORE_INFO("LOADING DONE");
				UINT32 leftover = (m_DataSize - m_SamplePosition) / m_DeviceProps.nBlockAlign;
				if (leftover < sampleBufferSize) sampleBufferSize = leftover;
			}
			LoadWAV(m_Data + m_SamplePosition, bufferData, sampleBufferSize);
			m_SamplePosition += m_DeviceProps.nAvgBytesPerSec;
		}
	}

	// TODO: Create switch cases for all sample sizes and formats  
	void Loader::LoadWAV(BYTE* bufferTarget, BYTE* bufferSrc, UINT32 totalSamples)
	{
		short int* srcBuffer = reinterpret_cast<short int*>(bufferSrc);
		float* targetBuffer = reinterpret_cast<float*>(bufferTarget);

		// Loop for Device with	32 bits (4 bytes) per sample     
		for (UINT32 i = 0; i < totalSamples; i+= m_DeviceProps.nChannels)
		{
			for (UINT32 j = 0; j < m_DeviceProps.nChannels; j++)
			{
				targetBuffer[i + j] = Audio::ConvertFloat(srcBuffer[i + j]) / SHORT_INT_MAX;
			}
		}
	}
}