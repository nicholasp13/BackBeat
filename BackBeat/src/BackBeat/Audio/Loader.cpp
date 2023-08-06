#include "bbpch.h"

#include "Audio.h"
#include "Loader.h"

/* README
* TODO: CREATE CONVERSION FOR 
*			DIFFERENT SAMPLE RATES
*			DIFFERENT BYTE/NUMBER FORMATS
*			MP3 FORMAT
*/
namespace BackBeat {

	Loader::Loader(tWAVEFORMATEX* deviceProps, AudioData* dataSrc)
		: m_DeviceProps(*deviceProps), m_Loading(false), m_DataSrc(dataSrc)
	{
		m_DataSize = dataSrc->GetSize() * deviceProps->nBlockAlign / dataSrc->GetProperties()->nBlockAlign;
		m_Data = new BYTE[m_DataSize];
	}

	Loader::~Loader()
	{

	}

	void Loader::Start()
	{
		m_Loading = true;
		BB_CORE_INFO("LOADING STARTED");
		Load();
	}

	void Loader::Stop()
	{
		m_Loading = false;
		BB_CORE_INFO("LOADING STOPPED");
	}

	HRESULT Loader::GetData(UINT32 framesAvailable, BYTE* buffer,
		UINT32* position, bool* playing)
	{
		UINT32 totalBytes = framesAvailable * m_DeviceProps.nBlockAlign;

		if (totalBytes + *position >= m_DataSize) totalBytes = m_DataSize - *position;
		
		memcpy(buffer, m_Data + *position, totalBytes);
		*position += totalBytes;

		if (*position >= m_DataSize) {
			*playing = FALSE;
		}

		return S_OK;
	}

	void Loader::Load()
	{	
		HRESULT hr = S_OK;
		UINT32 bytePosition = WAV_HEADER_SIZE;
		UINT32 samplePosition = WAV_HEADER_SIZE;	
		UINT32 bufferSize = m_DataSrc->GetProperties()->nAvgBytesPerSec;
		UINT32 sampleBufferSize = m_DataSrc->GetProperties()->nSamplesPerSec * m_DataSrc->GetProperties()->nChannels;
		BYTE* bufferData = new BYTE[bufferSize];

		while (m_Loading) {

			hr = m_DataSrc->LoadBuffer(bufferSize, bufferData, &bytePosition);
			CHECK_FAILURE(hr);

			if (bytePosition >= m_DataSrc->GetSize()) {
				m_Loading = false;
				int leftover = (m_DataSize - samplePosition) / m_DeviceProps.nBlockAlign;
				if (leftover < sampleBufferSize) sampleBufferSize = leftover;
			}

			LoadWAV(m_Data + samplePosition, bufferData, sampleBufferSize);
			samplePosition += m_DeviceProps.nAvgBytesPerSec;
		}
		BB_CORE_INFO("LOADING DONE");

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