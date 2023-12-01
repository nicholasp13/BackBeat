#include "bbpch.h"

#include "WindowsPlayer.h"
namespace BackBeat {

	// TODO: Make Loader a member
	//       Make DataManager class

	WindowsPlayer::WindowsPlayer()
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

	WindowsPlayer::~WindowsPlayer()
	{
		/**
		if (m_Loader->Loading)
			m_Loader->Stop();
		if (m_Worker.joinable())
			m_Worker.join();
		/**/
	}

	void WindowsPlayer::Play()
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

	void WindowsPlayer::Pause()
	{
		HRESULT hr = m_AudioClient->Stop();
		CHECK_FAILURE(hr);

		Playing = false;
		BB_CORE_INFO("PLAYING PAUSED");
	}

	void WindowsPlayer::Stop()
	{
		if (m_File->GetFileType() == FileType::WAV_FILE) m_Position = WAV_HEADER_SIZE;
		else m_Position = 0;

		Playing = false;
		BB_CORE_INFO("PLAYING STOPPED. POSITION RESET TO 0");
	}

	void WindowsPlayer::InitAudioClient() {
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

		hr = m_AudioClient->GetMixFormat(&m_WDeviceProps);
		CHECK_FAILURE(hr);
		
		hr = m_AudioClient->GetDevicePeriod(NULL, &bufferDuration);
		CHECK_FAILURE(hr);

		hr = m_AudioClient->Initialize(
			AUDCLNT_SHAREMODE_SHARED,
			0,
			0,
			0,
			m_WDeviceProps,
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
									/ m_WDeviceProps->nSamplesPerSec; 

		BB_CORE_INFO("---- WINDOWS DEVICE PROPERTIES --- ");
		BB_CORE_TRACE("Audio Format: {0}", m_WDeviceProps->wFormatTag);
		BB_CORE_TRACE("Number of Channels: {0}", m_WDeviceProps->nChannels);
		BB_CORE_TRACE("Sample Rate: {0}", m_WDeviceProps->nSamplesPerSec);
		BB_CORE_TRACE("Byte Rate: {0}", m_WDeviceProps->nAvgBytesPerSec);
		BB_CORE_TRACE("Block Align: {0}", m_WDeviceProps->nBlockAlign);
		BB_CORE_TRACE("Bits per Sample: {0}", m_WDeviceProps->wBitsPerSample);

		m_Props.bigEndian = Audio::IsBigEndian();
		m_Props.format = m_WDeviceProps->wFormatTag;
		m_Props.numChannels = m_WDeviceProps->nChannels;
		m_Props.sampleRate = m_WDeviceProps->nSamplesPerSec;
		m_Props.byteRate = m_WDeviceProps->nAvgBytesPerSec;
		m_Props.blockAlign = m_WDeviceProps->nBlockAlign;
		m_Props.bitDepth = m_WDeviceProps->wBitsPerSample;
		m_Props.fileSize = 0;

		BB_CORE_INFO("------ MY DEVICE PROPERTIES ------");
		if (m_Props.bigEndian)
			BB_CORE_TRACE("Big endian");
		else
			BB_CORE_TRACE("Little endian");
		BB_CORE_TRACE("Audio Format: {0}", m_Props.format);
		BB_CORE_TRACE("Number of Channels: {0}", m_Props.numChannels);
		BB_CORE_TRACE("Sample Rate: {0}", m_Props.sampleRate);
		BB_CORE_TRACE("Byte Rate: {0}", m_Props.byteRate);
		BB_CORE_TRACE("Block Align: {0}", m_Props.blockAlign);
		BB_CORE_TRACE("Bits per Sample: {0}", m_Props.bitDepth);
	}
}