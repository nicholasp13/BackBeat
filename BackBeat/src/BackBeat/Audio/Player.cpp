#include "bbpch.h"

#include "Player.h"
#include "BackBeat/Core/Core.h"
#include "FileReader.h"

#include <mmdeviceapi.h>
#include <Audioclient.h>
#include <AudioSessionTypes.h>

namespace BackBeat {

#define CHECK_FAILURE( hr ) \
	if (FAILED(hr)) \
	{ BB_CORE_ERROR("{0} FAILED TO INITIALIZE AUDIOCLIENT", hr); return; }

	Player::Player(std::string filePath)
	{
		m_FilePath = filePath;
		FileSelected = true;

		IAudioClient* m_AudioClient = NULL;
		IMMDeviceEnumerator* m_Enumerator = NULL;
		IMMDevice* m_Device = NULL;
		IAudioRenderClient* m_Renderer = NULL;
		
		InitAudioClient();
	}

	Player::~Player()
	{

	}

	void Player::Play()
	{
		
	}

	void Player::Pause()
	{

	}

	void Player::Stop()
	{

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

		const REFIID IID_IAudioRenderClient = __uuidof(IAudioRenderClient);
		hr = m_AudioClient->GetService(IID_IAudioRenderClient, (void**)&m_Renderer);

		CHECK_FAILURE(hr);

		BB_CORE_TRACE("DEVICE PROPERTIES");
		BB_CORE_INFO("Audio Format: {0}", m_DeviceProps->wFormatTag);
		BB_CORE_INFO("Number of Channels: {0}", m_DeviceProps->nChannels);
		BB_CORE_INFO("Sample Rate: {0}", m_DeviceProps->nSamplesPerSec);
		BB_CORE_INFO("Byte Rate: {0}", m_DeviceProps->nAvgBytesPerSec);
		BB_CORE_INFO("Block Align: {0}", m_DeviceProps->nBlockAlign);
		BB_CORE_INFO("Bits per Sample: {0}", m_DeviceProps->wBitsPerSample);
		BB_CORE_INFO("File Size: {0}", m_DeviceProps->cbSize);

		m_FileProps = new tWAVEFORMATEX;
		hr = FileReader::GetHeader(m_FilePath, m_FileProps);

		CHECK_FAILURE(hr);

		BB_CORE_TRACE("FILE PROPERTIES");
		BB_CORE_INFO("Audio Format: {0}", m_FileProps->wFormatTag);
		BB_CORE_INFO("Number of Channels: {0}", m_FileProps->nChannels);
		BB_CORE_INFO("Sample Rate: {0}", m_FileProps->nSamplesPerSec);
		BB_CORE_INFO("Byte Rate: {0}", m_FileProps->nAvgBytesPerSec);
		BB_CORE_INFO("Block Align: {0}", m_FileProps->nBlockAlign);
		BB_CORE_INFO("Bits per Sample: {0}", m_FileProps->wBitsPerSample);
		BB_CORE_INFO("File Size: {0}", m_FileProps->cbSize);
	}
}