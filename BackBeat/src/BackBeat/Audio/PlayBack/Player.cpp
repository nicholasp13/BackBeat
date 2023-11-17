#include "bbpch.h"

#include "Player.h"
namespace BackBeat {

	Player::Player()
		:
		m_Position(0),
		m_Worker{},
		m_AudioClient(NULL),
		m_Enumerator(NULL),
		m_Device(NULL),
		m_Renderer(NULL),
		m_File(NULL),
		m_Loader(NULL)
	{
		InitAudioClient();
	}

	Player::~Player()
	{
		if (m_Loader->Loading)
			m_Loader->Stop();
		if (m_Worker.joinable())
			m_Worker.join();
	}

	void Player::Play()
	{
		if (!m_Loader) {
			BB_CORE_ERROR("SET LOADER BEFORE PLAYING");
			return;
		}
		if (Playing) {
			BB_CORE_ERROR("ALREADY PLAYING");
			return;
		}
		
		HRESULT hr;
		UINT32 padding;
		UINT32 framesAvailable;
		BYTE* data;
		DWORD flags = 0;
		DWORD sleepTime = (DWORD)(m_ActualBufferDuration / REFTIMES_PER_MILLISEC / 10);

		if (!m_Loader->Loading && !m_Loader->Loaded)
			m_Worker = std::thread(&Loader::Start, m_Loader);

		Playing = true;
		framesAvailable = m_BufferSize;

		// Pre-call the buffer to avoid bugs on start
		hr = m_AudioClient->GetCurrentPadding(&padding);
		CHECK_FAILURE(hr);

		framesAvailable = m_BufferSize - padding;
		hr = m_Renderer->GetBuffer(framesAvailable, &data);

		CHECK_FAILURE(hr);

		hr = m_Loader->GetData(framesAvailable, data, &m_Position, &Playing);
		CHECK_FAILURE(hr);

		hr = m_Renderer->ReleaseBuffer(framesAvailable, flags);
		CHECK_FAILURE(hr);

		hr = m_AudioClient->Start();
		CHECK_FAILURE(hr);

		BB_CORE_INFO("PLAYING STARTED");

		while (Playing && (flags != AUDCLNT_BUFFERFLAGS_SILENT) )
		{
			Sleep(sleepTime);

			hr = m_AudioClient->GetCurrentPadding(&padding);
			CHECK_FAILURE(hr);

			framesAvailable = m_BufferSize - padding;

			hr = m_Renderer->GetBuffer(framesAvailable, &data);
			CHECK_FAILURE(hr);

			hr = m_Loader->GetData(framesAvailable, data, &m_Position, &Playing);
			CHECK_FAILURE(hr);

			hr = m_Renderer->ReleaseBuffer(framesAvailable, flags);
			CHECK_FAILURE(hr);
		}
		Sleep(sleepTime);
		Playing = false;

		hr = m_AudioClient->Stop();
		CHECK_FAILURE(hr);

		BB_CORE_INFO("PLAYING DONE");
	}

	// Might not be needed
	void Player::PlaySamples(UINT32 samples) {
		if (!Playing) {
			BB_CORE_INFO("SET TO PLAY BEFORE CALL");
			return;
		}

		if (samples < 1 || samples > m_File->GetSize() / m_DeviceProps->nBlockAlign) {
			BB_CORE_ERROR("INVALID NUMBER OF SAMPLES");
			return;
		}

		HRESULT hr;
		UINT32 padding;
		UINT32 framesAvailable;
		BYTE* data;
		DWORD flags = 0;
		DWORD sleepTime = (DWORD)(m_ActualBufferDuration / REFTIMES_PER_MILLISEC / 2);

		if (!m_Loader->Loading && !m_Loader->Loaded) {
			std::thread worker(&Loader::Start, m_Loader);
			worker.detach();
		}

		Playing = true;
		framesAvailable = m_BufferSize;


		// Pre-call the buffer to avoid bugs on start
		hr = m_AudioClient->GetCurrentPadding(&padding);
		CHECK_FAILURE(hr);

		framesAvailable = m_BufferSize - padding;
		hr = m_Renderer->GetBuffer(framesAvailable, &data);

		CHECK_FAILURE(hr);

		hr = m_Loader->GetData(framesAvailable, data, &m_Position, &Playing);
		CHECK_FAILURE(hr);

		hr = m_Renderer->ReleaseBuffer(framesAvailable, flags);
		CHECK_FAILURE(hr);

		hr = m_AudioClient->Start();
		CHECK_FAILURE(hr);

		BB_CORE_INFO("PLAYING SAMPLES");

		for(UINT32 i = 0; i < samples; i++)
		{
			Sleep(sleepTime);

			hr = m_AudioClient->GetCurrentPadding(&padding);
			CHECK_FAILURE(hr);

			framesAvailable = m_BufferSize - padding;

			hr = m_Renderer->GetBuffer(framesAvailable, &data);
			CHECK_FAILURE(hr);

			hr = m_Loader->GetData(framesAvailable, data, &m_Position, &Playing);
			CHECK_FAILURE(hr);

			hr = m_Renderer->ReleaseBuffer(framesAvailable, flags);
			CHECK_FAILURE(hr);

			if (!Playing) {
				break;
			}
		}
		Sleep(sleepTime);
		Playing = false;

		hr = m_AudioClient->Stop();
		CHECK_FAILURE(hr);

		BB_CORE_INFO("PLAYING DONE");
	}

	void Player::Pause()
	{
		HRESULT hr = m_AudioClient->Stop();
		CHECK_FAILURE(hr);

		Playing = false;
		BB_CORE_INFO("PLAYING PAUSED");
	}

	void Player::Stop()
	{
		if (m_File->GetFileType() == FileType::WAV_FILE) m_Position = WAV_HEADER_SIZE;
		else m_Position = 0;

		Playing = false;
		BB_CORE_INFO("PLAYING STOPPED. POSITION RESET TO 0");
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

		BB_CORE_INFO("BUFFER SIZE: {0}", m_BufferSize);

		REFIID IID_IAudioRenderClient = __uuidof(IAudioRenderClient);
		hr = m_AudioClient->GetService(IID_IAudioRenderClient, (void**)&m_Renderer);
		CHECK_FAILURE(hr);

		m_ActualBufferDuration = (REFERENCE_TIME)bufferDuration * m_BufferSize
									/ m_DeviceProps->nSamplesPerSec; 

		BB_CORE_INFO("DEVICE PROPERTIES");
		BB_CORE_TRACE("Audio Format: {0}", m_DeviceProps->wFormatTag);
		BB_CORE_TRACE("Number of Channels: {0}", m_DeviceProps->nChannels);
		BB_CORE_TRACE("Sample Rate: {0}", m_DeviceProps->nSamplesPerSec);
		BB_CORE_TRACE("Byte Rate: {0}", m_DeviceProps->nAvgBytesPerSec);
		BB_CORE_TRACE("Block Align: {0}", m_DeviceProps->nBlockAlign);
		BB_CORE_TRACE("Bits per Sample: {0}", m_DeviceProps->wBitsPerSample);

	}
}