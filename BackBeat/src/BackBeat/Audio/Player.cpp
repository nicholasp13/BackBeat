#include "bbpch.h"

#include "Player.h"
#include "BackBeat/Core/Core.h"
#include "FileReader.h"

#include <mmdeviceapi.h>
#include <Audioclient.h>
#include <AudioSessionTypes.h>
#include <synchapi.h>

namespace BackBeat {

#define BYTESIZE 8

// REFERENCE_TIME time units per second and per millisecond
#define REFTIMES_PER_SECOND 10000000
#define REFTIMES_PER_MILLISEC 10000


#define CHECK_FAILURE( hr ) \
	if (FAILED(hr)) \
	{ BB_CORE_ERROR("{0} FAILED TO INITIALIZE AUDIOCLIENT", hr); FileOpened = false; return; }

#define FILE_OPENED( fileOpened ) \
	if (!fileOpened) \
	{ BB_CORE_ERROR("FAILED TO OPEN FILE"); FileOpened = false; Playing = false; return; }

	Player::Player(std::string filePath)
	{
		m_FilePath = filePath;
		FileOpened = true;

		IAudioClient* m_AudioClient = NULL;
		IMMDeviceEnumerator* m_Enumerator = NULL;
		IMMDevice* m_Device = NULL;
		IAudioRenderClient* m_Renderer = NULL;
		
		InitAudioClient();
	}

	// TODO: Create destructor to release all data
	Player::~Player()
	{

	}

	void Player::Play()
	{
		FILE_OPENED(FileOpened);

		if (Playing)
		{
			BB_CORE_ERROR("ALREADY PLAYING");
			return;
		}
		
		HRESULT hr;
		UINT32 padding;
		UINT32 framesAvailable;
		DWORD flags = 0;
		DWORD sleepTime = (DWORD)(m_ActualBufferDuration / REFTIMES_PER_MILLISEC / 2);
		std::ifstream file;

		Playing = true;

		hr = m_AudioClient->Start();

		CHECK_FAILURE(hr);

		// Create File Reader for whole file
		while (Playing && (flags != AUDCLNT_BUFFERFLAGS_SILENT))
		{
			Sleep(sleepTime);

			hr = m_AudioClient->GetCurrentPadding(&padding);
			
			CHECK_FAILURE(hr);

			framesAvailable = m_BufferSize - padding;

			hr = m_Renderer->GetBuffer(framesAvailable, &m_Data);

			CHECK_FAILURE(hr);
			
			// TODO: CREATE WAY TO GET/READ Audio files

			hr = m_Renderer->ReleaseBuffer(framesAvailable, flags);

			CHECK_FAILURE(hr)
		}
		Sleep(sleepTime);

		Playing = false;
		file.close();
	}

	void Player::Pause()
	{
		FILE_OPENED(FileOpened);

		Playing = false;
		BB_CORE_INFO("AUDIO PAUSED");
	}

	void Player::Stop()
	{
		FILE_OPENED(FileOpened);

		Playing = false;
		BB_CORE_INFO("AUDIO STOPPED. POSITION RESET TO 0");
	}

	void Player::InitAudioClient() {
		HRESULT hr;
		REFERENCE_TIME bufferDuration = 0;

		hr = CoInitializeEx(NULL, COINITBASE_MULTITHREADED);

		CHECK_FAILURE(hr);

		const CLSID CLSID_MMDeviceEnumerator = __uuidof(MMDeviceEnumerator);
		const IID IID_IMMDeviceEnumerator = __uuidof(IMMDeviceEnumerator);
		hr = CoCreateInstance(
			CLSID_MMDeviceEnumerator, NULL,
			CLSCTX_ALL, IID_IMMDeviceEnumerator,
			(void**)&m_Enumerator);

		CHECK_FAILURE(hr);

		hr = m_Enumerator->GetDefaultAudioEndpoint(eRender, eMultimedia, &m_Device);

		CHECK_FAILURE(hr);

		const IID IID_IAudioClient = __uuidof(IAudioClient);
		hr = m_Device->Activate(IID_IAudioClient, CLSCTX_ALL, NULL, (void**)&m_AudioClient);

		CHECK_FAILURE(hr);

		hr = m_AudioClient->GetMixFormat(&m_DeviceProps);

		CHECK_FAILURE(hr);
		
		hr = m_AudioClient->GetDevicePeriod(NULL, &bufferDuration);

		CHECK_FAILURE(hr);

		/* TODO: CREATE A FUNCTION TO ALLOW FOR EXCLUSIVE MODE IF POSSIBLE */
		hr = m_AudioClient->Initialize(
			AUDCLNT_SHAREMODE_SHARED,
			0,
			bufferDuration,
			0,
			m_DeviceProps,
			NULL
			);

		CHECK_FAILURE(hr);

		hr = m_AudioClient->GetBufferSize(&m_BufferSize);

		CHECK_FAILURE(hr);

		const REFIID IID_IAudioRenderClient = __uuidof(IAudioRenderClient);
		hr = m_AudioClient->GetService(IID_IAudioRenderClient, (void**)&m_Renderer);

		CHECK_FAILURE(hr);

		m_Data = new BYTE[m_BufferSize];
		m_ActualBufferDuration = (double)REFTIMES_PER_SECOND * m_BufferSize
									/ m_DeviceProps->nSamplesPerSec;

		BB_CORE_INFO("DEVICE PROPERTIES");
		BB_CORE_TRACE("Audio Format: {0}", m_DeviceProps->wFormatTag);
		BB_CORE_TRACE("Number of Channels: {0}", m_DeviceProps->nChannels);
		BB_CORE_TRACE("Sample Rate: {0}", m_DeviceProps->nSamplesPerSec);
		BB_CORE_TRACE("Byte Rate: {0}", m_DeviceProps->nAvgBytesPerSec);
		BB_CORE_TRACE("Block Align: {0}", m_DeviceProps->nBlockAlign);
		BB_CORE_TRACE("Bits per Sample: {0}", m_DeviceProps->wBitsPerSample);
		BB_CORE_TRACE("File Size: {0}", m_DeviceProps->cbSize);

		// TODO: Chanfe to create file
		// hr = FileReader::CreateFile(m_FilePath, m_FileProps);

		CHECK_FAILURE(hr);

		BB_CORE_INFO("FILE PROPERTIES");
		BB_CORE_TRACE("Audio Format: {0}", m_FileProps->wFormatTag);
		BB_CORE_TRACE("Number of Channels: {0}", m_FileProps->nChannels);
		BB_CORE_TRACE("Sample Rate: {0}", m_FileProps->nSamplesPerSec);
		BB_CORE_TRACE("Byte Rate: {0}", m_FileProps->nAvgBytesPerSec);
		BB_CORE_TRACE("Block Align: {0}", m_FileProps->nBlockAlign);
		BB_CORE_TRACE("Bits per Sample: {0}", m_FileProps->wBitsPerSample);
		BB_CORE_TRACE("File Size: {0}", m_FileProps->cbSize);
	}
}