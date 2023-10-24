#include "bbpch.h"

#include "WindowsRenderer.h"
namespace BackBeat {

	WindowsRenderer::WindowsRenderer(tWAVEFORMATEX props, std::shared_ptr<RenderInfo> info, std::shared_ptr<AudioEngine> engine)
		:
		m_Rendering(false),
		m_SynthProps(props),
		m_Info(info),
		m_Engine(engine),
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

	}

	void WindowsRenderer::Render()
	{
		m_Rendering = true;
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

		GetData(data, framesAvailable);

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

			GetData(data, framesAvailable);

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
	}

	void WindowsRenderer::GetData(BYTE* data, UINT32 framesAvailable)
	{
		m_Info->SetSamplesToRender(framesAvailable);
		m_Engine->Render(m_Info);

		float* targetBuffer = reinterpret_cast<float*>(data);
		std::shared_ptr<float[]> srcBuffer = m_Info->GetBuffer();

		for (UINT32 i = 0; i < framesAvailable * m_SynthProps.nChannels; i += m_SynthProps.nChannels)
		{
			for (UINT32 j = 0; j < m_SynthProps.nChannels; j++)
			{
				targetBuffer[i + j] = srcBuffer[i + j];
			}
		}
	}
}