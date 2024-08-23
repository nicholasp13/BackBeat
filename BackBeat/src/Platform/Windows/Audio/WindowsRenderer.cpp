#include "bbpch.h"

// TODO: 
//     - Fix bug where changing the audio output device on PC stops audio from rendering 
//       by at least warning the user (this happens at the error Window API error message)

// NOTE: Setting a different output device on your Windows PC changes the props

#include "BackBeat/Audio/Helpers/SpinSleeper.h"
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

		hr = m_AudioClient->Initialize(
			AUDCLNT_SHAREMODE_SHARED,
			0,
			bufferDuration,
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
	}

	void WindowsRenderer::Stop() 
	{
		if (!m_Init)
			return;
		m_Rendering = false;
	}

	// NOTE: Have tried multiple different workflows with WASAPI and have found that trying to time Render() outside this
	//       class/scope causes more trouble for what its worth in trying to sync the audio to minimize artifacting. 
	//       Have found that interupting the WASAPI function calls even a little creates artifacting that would require
	//       advanced synchronizing that ultimately is either impossible with current knowledge or not worth the benefit
	//       of a larger seperation between BackBeat and WASAPI, and the theoretical boost in performance by seperating the threads.
	void WindowsRenderer::Render()
	{
		BB_CORE_ERROR("WindowsRenderer ERROR: SHOULD BE RENDERED FREELY NOT IN A LOOP OUTSIDE OF THIS CLASS/SCOPE");
	}

	void WindowsRenderer::RenderFree()
	{
		SpinSleeper spinner;
		Timer timer;
		float time = 0.0f;
		float lastTimeFrame = 0.0f;
		float deltaTime = 0.0f;

		HRESULT hr = S_OK;
		UINT32 padding = 0;
		INT32 position = 0;
		DWORD flags = 0;
		UINT32 framesAvailable = m_BufferSize;
		nanoseconds sleepTime = (nanoseconds)(m_ActualBufferDuration) / Windows::ReftimesPerMillisecond / 2 * 1000000;
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

		timer.Start();
		lastTimeFrame = timer.GetTimeNano();
		while (m_Rendering)
		{
			time = timer.GetTimeNano();
			spinner.Spin(sleepTime - nanoseconds(time));
			timer.Reset();

			hr = m_AudioClient->GetCurrentPadding(&padding);
			CHECK_FAILURE_HRESULT(hr);

			framesAvailable = m_BufferSize - padding;

			hr = m_ClientRenderer->GetBuffer(framesAvailable, &data);
			CHECK_FAILURE_HRESULT(hr);

			m_Mixer->RenderData(data, framesAvailable);

			hr = m_ClientRenderer->ReleaseBuffer(framesAvailable, flags);
			CHECK_FAILURE_HRESULT(hr);
		}

		hr = m_AudioClient->Stop();
		CHECK_FAILURE_HRESULT(hr);

		return; 

	FailureExit:
		m_Rendering = false;
	}

}