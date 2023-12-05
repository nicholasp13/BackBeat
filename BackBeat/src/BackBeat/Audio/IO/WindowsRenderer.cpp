#include "bbpch.h"

// TODO: 
//     Add support for other file formats.
//     Fix bug where changing the audio output device on PC stops audio from rendering

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
		HRESULT hr;
		UINT32 padding;
		UINT32 framesAvailable;
		INT32 position = 0;
		DWORD flags = 0;
		DWORD sleepTime = (DWORD)(m_ActualBufferDuration / REFTIMES_PER_MILLISEC / 2);
		BYTE* data;

		framesAvailable = m_BufferSize;

		hr = m_AudioClient->GetCurrentPadding(&padding);
		CHECK_FAILURE(hr);

		framesAvailable = (m_BufferSize - padding);

		hr = m_ClientRenderer->GetBuffer(framesAvailable, &data);
		CHECK_FAILURE(hr);

		m_Mixer->GetData(data, framesAvailable); //  TODO: Reimplement way to get data after creating mixer

		hr = m_ClientRenderer->ReleaseBuffer(framesAvailable, flags);
		CHECK_FAILURE(hr);

		hr = m_AudioClient->Start();
		CHECK_FAILURE(hr);

		while (m_Rendering)
		{
			Sleep(sleepTime);

			hr = m_AudioClient->GetCurrentPadding(&padding);
			CHECK_FAILURE(hr);

			framesAvailable = m_BufferSize - padding;

			hr = m_ClientRenderer->GetBuffer(framesAvailable, &data);
			CHECK_FAILURE(hr);

			m_Mixer->GetData(data, framesAvailable);

			hr = m_ClientRenderer->ReleaseBuffer(framesAvailable, flags);
			CHECK_FAILURE(hr);

		}
		Sleep(sleepTime);

		hr = m_AudioClient->Stop();
		CHECK_FAILURE(hr);
	}

	void WindowsRenderer::InitRenderer()
	{
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

		hr = m_AudioClient->Initialize(
			AUDCLNT_SHAREMODE_SHARED,
			0,
			0,
			0,
			m_DeviceProps,
			NULL
		);
		CHECK_FAILURE(hr);

		hr = m_AudioClient->GetBufferSize(&m_BufferSize);
		CHECK_FAILURE(hr);

		REFIID IID_IAudioRenderClient = __uuidof(IAudioRenderClient);

		hr = m_AudioClient->GetService(IID_IAudioRenderClient, (void**)&m_ClientRenderer);
		CHECK_FAILURE(hr);

		m_ActualBufferDuration = (REFERENCE_TIME)bufferDuration * m_BufferSize
			/ m_DeviceProps->nSamplesPerSec;

		m_Props.bigEndian = Audio::IsBigEndian();
		m_Props.format = m_DeviceProps->wFormatTag;
		m_Props.numChannels = m_DeviceProps->nChannels;
		m_Props.sampleRate = m_DeviceProps->nSamplesPerSec;
		m_Props.byteRate = m_DeviceProps->nAvgBytesPerSec;
		m_Props.blockAlign = m_DeviceProps->nBlockAlign;
		m_Props.bitDepth = m_DeviceProps->wBitsPerSample;
		m_Props.fileSize = 0;

		m_Mixer = std::make_shared<Mixer>(m_Props);
	}

}