#include "bbpch.h"

#include "Player.h"
#include "BackBeat/Core/Core.h"
#include "FileReader.h"

#include <mmdeviceapi.h>
#include <Audioclient.h>

namespace BackBeat {

	HRESULT hr = S_OK;

	Player::Player()
	{
		hr = CoInitializeEx(NULL, COINITBASE_MULTITHREADED);

		const CLSID CLSID_MMDeviceEnumerator = __uuidof(MMDeviceEnumerator);
		const IID IID_IMMDeviceEnumerator = __uuidof(IMMDeviceEnumerator);
		hr = CoCreateInstance(
			CLSID_MMDeviceEnumerator, NULL,
			CLSCTX_ALL, IID_IMMDeviceEnumerator,
			(void**)&m_Enumerator);

		if (hr != S_OK) 
		{
			BB_CORE_ERROR("{0} FAILED TO GET ENUMERATOR", hr);
		}
		else { BB_CORE_TRACE("Successfully got ENUMERATOR!"); }

		hr = m_Enumerator->GetDefaultAudioEndpoint(eRender, eMultimedia, &m_Device);
		
		if (hr != S_OK)
		{
			BB_CORE_ERROR("{0} FAILED TO GET DEVICE", hr);
		}
		else { BB_CORE_TRACE("Successfully got DEVICE!"); }
		
		const IID IID_IAudioClient = __uuidof(IAudioClient);
		hr = m_Device->Activate(IID_IAudioClient, CLSCTX_ALL, NULL, (void**)&m_AudioClient);
		
		if (hr != S_OK) 
		{
			BB_CORE_ERROR("{0} FAILED TO GET AUDIOCLIENT", hr);
		}
		else { BB_CORE_TRACE("Successfully got AUDIOCLIENT!"); }
		
		hr = m_AudioClient->GetBufferSize(&m_BufferSize);

		if (hr != S_OK)
		{
			BB_CORE_ERROR("{0} FAILED TO GET BUFFERSIZE", hr);
		}
		else { BB_CORE_TRACE("Successfully got BUFFERSIZE!"); }

		/* TODO: CREATE INITIALIZE AFTER CREATING MP3 format reader
		m_AudioClient->Initialize(
			AUDCLNT_SHAREMODE_EXCLUSIVE, 0,

			);

		const REFIID IID_IAudioRenderClient = __uuidof(IAudioRenderClient);
		hr = m_AudioClient->GetService(IID_IAudioRenderClient, (void**)&m_Renderer);
		
		if (hr != S_OK)
		{
			BB_CORE_ERROR("{0} FAILED TO GET AUDIO RENDERER", hr);
		}
		else { BB_CORE_TRACE("Successfully got AUDIOCLIENT!"); }
		*/
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

	void Player::SetFile()
	{

	}
}