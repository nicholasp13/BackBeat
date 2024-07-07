#include "bbpch.h"

// TODO: 
//     - Delete Window API members in deconstructor
//     - Fix bug where changing the audio output device on PC stops audio from rendering 
//       by at least warning the user (this happens at the error Window API error message)

// NOTE: Setting a different output device on your Windows PC changes the props

#include "WindowsRenderer.h"
namespace BackBeat {

	WindowsRenderer::WindowsRenderer()
		:
		m_Rendering(false),
		m_BufferSize(0),
		m_AudioClient(NULL),
		m_Enumerator(NULL),
		m_Device(NULL),
		m_ClientRenderer(NULL)
	{
		InitRenderer();
	}

	WindowsRenderer::~WindowsRenderer()
	{
		Stop();
	}

	void WindowsRenderer::Start()
	{
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

		m_Mixer->GetData(data, framesAvailable);

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

			m_Mixer->GetData(data, framesAvailable);

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

	void WindowsRenderer::InitRenderer()
	{
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

		m_ActualBufferDuration = (REFERENCE_TIME)bufferDuration * m_BufferSize
			/ m_DeviceProps->nSamplesPerSec;

		// Refactor into a function
		m_Props.bigEndian = Audio::IsBigEndian();
		m_Props.format = m_DeviceProps->wFormatTag;
		m_Props.numChannels = m_DeviceProps->nChannels;
		m_Props.sampleRate = m_DeviceProps->nSamplesPerSec;
		m_Props.byteRate = m_DeviceProps->nAvgBytesPerSec;
		m_Props.blockAlign = m_DeviceProps->nBlockAlign;
		m_Props.bitDepth = m_DeviceProps->wBitsPerSample;
		m_Props.fileSize = 0;

		m_Mixer = std::make_shared<Mixer>(m_Props);

	// NOTE: This is here to make sure CHECK_FAILURE macro works. Code will be added if needed to avoid bugs when
	//       a Windows API error occurs i.e. audio does not work if any of the checks fail so there needs to be a
	//       way to reset it
	FailureExit:
		;
	}

}