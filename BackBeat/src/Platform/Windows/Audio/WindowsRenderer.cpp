#include "bbpch.h"

// TODO: 
//     - Fix bug where changing the audio output device on PC stops audio from rendering 
//       by at least warning the user (this happens at the error Window API error message)

// NOTE: Setting a different output device on your Windows PC changes the props

#include "WindowsRenderer.h"
namespace BackBeat {

	WindowsRenderer::WindowsRenderer()
		:
		m_Rendering(false),
		m_Init(false),
		m_BufferSize(0),
		m_Props(AudioProps()),
		m_Mixer(nullptr),
		m_ActualBufferDuration(REFERENCE_TIME(0)),
		m_AudioClient(NULL),
		m_Enumerator(NULL),
		m_Device(NULL),
		m_ClientRenderer(NULL),
		m_DeviceProps(NULL)
	{
		
	}

	WindowsRenderer::~WindowsRenderer()
	{
		Stop();
		ReleaseAll();
	}

	void WindowsRenderer::ReleaseAll()
	{
		m_Init = false;
		CoTaskMemFree(m_DeviceProps);
		if (m_Enumerator != NULL)
			m_Enumerator->Release();
		if (m_Device != NULL)
			m_Device->Release();
		if (m_AudioClient != NULL)
			m_AudioClient->Release();
		if (m_ClientRenderer != NULL)
			m_ClientRenderer->Release();
	}

	void WindowsRenderer::Init()
	{
		if (m_Init)
			return;

		HRESULT hr;
		REFERENCE_TIME bufferDuration = 0;

		hr = CoInitializeEx(NULL, COINITBASE_MULTITHREADED);
		CHECK_FAILURE_HRESULT(hr);

		hr = CoCreateInstance(
			Windows::CLSID_MMDeviceEnumerator,
			NULL,
			CLSCTX_ALL,
			Windows::IID_IMMDeviceEnumerator,
			(void**)&m_Enumerator);
		CHECK_FAILURE_HRESULT(hr);

		hr = m_Enumerator->GetDefaultAudioEndpoint(eRender, eMultimedia, &m_Device);
		CHECK_FAILURE_HRESULT(hr);

		hr = m_Device->Activate(Windows::IID_IAudioClient, CLSCTX_ALL, NULL, (void**)&m_AudioClient);
		CHECK_FAILURE_HRESULT(hr);

		hr = m_AudioClient->GetMixFormat(&m_DeviceProps);
		CHECK_FAILURE_HRESULT(hr);

		hr = m_AudioClient->GetDevicePeriod(NULL, &bufferDuration);
		CHECK_FAILURE_HRESULT(hr);

		hr = m_AudioClient->Initialize(
			AUDCLNT_SHAREMODE_SHARED,
			0,
			0,
			0,
			m_DeviceProps,
			NULL
		);
		CHECK_FAILURE_HRESULT(hr);

		hr = m_AudioClient->GetBufferSize(&m_BufferSize);
		CHECK_FAILURE_HRESULT(hr);

		hr = m_AudioClient->GetService(Windows::IID_IAudioRenderClient, (void**)&m_ClientRenderer);
		CHECK_FAILURE_HRESULT(hr);

		// Sets other members after Windows members are successfuly initialized
		m_Init = true;
		m_ActualBufferDuration = (REFERENCE_TIME)bufferDuration * m_BufferSize
			/ m_DeviceProps->nSamplesPerSec;
		m_Props.bigEndian = Audio::IsBigEndian();
		m_Props.fileSize = 0;
		Windows::AudioPropsConversion(m_DeviceProps, &m_Props);

		return;

	FailureExit:
		ReleaseAll();
	}

	void WindowsRenderer::Start()
	{
		if (!m_Init)
			return;

		m_Rendering = true;
		m_Thread = std::thread(&WindowsRenderer::Render, this);
	}

	void WindowsRenderer::Stop() 
	{
		m_Rendering = false; 
		if (m_Thread.joinable())
			m_Thread.join();
	}

	void WindowsRenderer::Render()
	{
		HRESULT hr = S_OK;
		UINT32 padding = 0;
		INT32 position = 0;
		DWORD flags = 0;
		UINT32 framesAvailable = m_BufferSize;
		DWORD sleepTime = (DWORD)(m_ActualBufferDuration / Windows::ReftimesPerMillisecond / 2);
		BYTE* data = nullptr;

		hr = m_AudioClient->GetCurrentPadding(&padding);
		CHECK_FAILURE_HRESULT(hr);

		framesAvailable = (m_BufferSize - padding);

		hr = m_ClientRenderer->GetBuffer(framesAvailable, &data);
		CHECK_FAILURE_HRESULT(hr);

		m_Mixer->RenderData(data, framesAvailable);

		hr = m_ClientRenderer->ReleaseBuffer(framesAvailable, flags);
		CHECK_FAILURE_HRESULT(hr);

		hr = m_AudioClient->Start();
		CHECK_FAILURE_HRESULT(hr);

		while (m_Rendering)
		{
			Sleep(sleepTime);

			hr = m_AudioClient->GetCurrentPadding(&padding);
			CHECK_FAILURE_HRESULT(hr);

			framesAvailable = m_BufferSize - padding;

			hr = m_ClientRenderer->GetBuffer(framesAvailable, &data);
			CHECK_FAILURE_HRESULT(hr);

			m_Mixer->RenderData(data, framesAvailable);

			hr = m_ClientRenderer->ReleaseBuffer(framesAvailable, flags);
			CHECK_FAILURE_HRESULT(hr);

		}
		Sleep(sleepTime);

		hr = m_AudioClient->Stop();
		CHECK_FAILURE_HRESULT(hr);

		return; 

	FailureExit:
		m_Rendering = false;
	}

}