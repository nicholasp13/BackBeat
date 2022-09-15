#include "bbpch.h"

#include "Player.h"
#include "BackBeat/Core/Core.h"

#include <mmdeviceapi.h>
#include <Audioclient.h>

namespace BackBeat {

	
	
	HRESULT hr = S_OK;

	Player::Player()
	{
		hr = CoInitializeEx(NULL, COINITBASE_MULTITHREADED);

		const CLSID CLSID_MMDeviceEnumerator = __uuidof(MMDeviceEnumerator);
		const IID IID_IMMDeviceEnumerator = __uuidof(IMMDeviceEnumerator);
		const IID IID_IAudioClient = __uuidof(IAudioClient);
		hr = CoCreateInstance(
			CLSID_MMDeviceEnumerator, NULL,
			CLSCTX_ALL, IID_IMMDeviceEnumerator,
			(void**)&m_Enumerator);

		if (hr != S_OK) 
		{
			BB_CORE_ERROR("{0} FAILED TO GET ENUMERATOR", hr);
		}
		BB_CORE_TRACE("Successfully got ENUMERATOR!");

		hr = m_Enumerator->GetDefaultAudioEndpoint(eRender, eMultimedia, &device);

		if (hr != S_OK)
		{
			BB_CORE_ERROR("{0} FAILED TO GET DEVICE", hr);
		}
		BB_CORE_TRACE("Successfully got DEVICE!");
		
		hr = device->Activate(IID_IAudioClient, CLSCTX_ALL, NULL, (void**)&m_AudioClient);

		if (hr != S_OK) 
		{
			BB_CORE_ERROR("{0} FAILED TO GET AUDIOCLIENT", hr);
		}
		BB_CORE_TRACE("Successfully got AUDIOCLIENT!");
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
}