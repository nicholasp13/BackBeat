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
		tWAVEFORMATEX* m_FileProps = NULL;
		
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

		hr = m_AudioClient->GetMixFormat(&m_FileProps);

		CHECK_FAILURE(hr);
		
		hr = m_AudioClient->GetDevicePeriod(NULL, &bufferDuration);

		CHECK_FAILURE(hr);

		/* TODO: CREATE A FUNCTION TO ALLOW FOR EXCLUSIVE MODE IF POSSIBLE */
		hr = m_AudioClient->Initialize(
			AUDCLNT_SHAREMODE_SHARED,
			0,
			bufferDuration,
			0,
			m_FileProps,
			NULL
			);

		if (hr == AUDCLNT_E_BUFFER_SIZE_NOT_ALIGNED)
		{
			BB_CORE_ERROR("AUDIO ENDPOINT BUFFER NOT ALIGNED");
		}
		CHECK_FAILURE(hr);

		const REFIID IID_IAudioRenderClient = __uuidof(IAudioRenderClient);
		hr = m_AudioClient->GetService(IID_IAudioRenderClient, (void**)&m_Renderer);

		CHECK_FAILURE(hr);
	}
}