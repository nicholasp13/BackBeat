#include "bbpch.h"

#include "Audio.h"
#include "Player.h"
#include "FileReader.h"
#include "AudioData.h"
#include "Loader.h"

/* README - TODO: 
*		CREATE A FUNCTION TO ALLOW FOR EXCLUSIVE MODE IF POSSIBLE
*		DELETE/FREE ALL POINTERS/HEAP DATA
*		MULTITHREADING
*/
namespace BackBeat {

	Player::Player(std::string filePath)
	{
		m_FilePath = filePath;
		m_Position = 0;

		m_AudioClient = NULL;
		m_Enumerator = NULL;
		m_Device = NULL;
		m_Renderer = NULL;
		m_File = NULL;
		
		InitAudioClient();
	}

	// TODO: Create destructor to release all data
	Player::~Player()
	{

	}

	void Player::Play()
	{

		if (Playing)
		{
			BB_CORE_ERROR("ALREADY PLAYING");
			return;
		}
		
		HRESULT hr;
		UINT32 padding;
		UINT32 framesAvailable;
		BYTE* data;
		DWORD flags = 0;
		DWORD sleepTime = (DWORD)(m_ActualBufferDuration / REFTIMES_PER_MILLISEC / 2);
		Loader loader = Loader(m_DeviceProps, m_File);

		std::thread worker(&Loader::Start, loader);
		Playing = true;
		framesAvailable = m_BufferSize;
		
		// Pre-call the buffer to avoid bugs on start
		hr = m_Renderer->GetBuffer(framesAvailable, &data);
		CHECK_FAILURE(hr);

		hr = loader.GetData(framesAvailable, data, &m_Position, &Playing);
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

			hr = loader.GetData(framesAvailable, data, &m_Position, &Playing);
			CHECK_FAILURE(hr);

			hr = m_Renderer->ReleaseBuffer(framesAvailable, flags);
			CHECK_FAILURE(hr);
		}
		Sleep(sleepTime);
		Playing = false;
		worker.join();
		hr = m_AudioClient->Stop();
		CHECK_FAILURE(hr);

		BB_CORE_INFO("PLAYING DONE");
	}

	void Player::Pause()
	{
		Playing = false;
		BB_CORE_INFO("AUDIO PAUSED");
	}

	void Player::Stop()
	{
		if (m_File->GetFileType() == FileType::WAV_FILE) m_Position = WAV_HEADER_SIZE;
		else m_Position = 0;

		Playing = false;
		BB_CORE_INFO("AUDIO STOPPED. POSITION RESET TO 0");
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
			REFTIMES_PER_SECOND,
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

		m_ActualBufferDuration = (double)REFTIMES_PER_SECOND * m_BufferSize
									/ m_DeviceProps->nSamplesPerSec; 

		BB_CORE_INFO("DEVICE PROPERTIES");
		BB_CORE_TRACE("Audio Format: {0}", m_DeviceProps->wFormatTag);
		BB_CORE_TRACE("Number of Channels: {0}", m_DeviceProps->nChannels);
		BB_CORE_TRACE("Sample Rate: {0}", m_DeviceProps->nSamplesPerSec);
		BB_CORE_TRACE("Byte Rate: {0}", m_DeviceProps->nAvgBytesPerSec);
		BB_CORE_TRACE("Block Align: {0}", m_DeviceProps->nBlockAlign);
		BB_CORE_TRACE("Bits per Sample: {0}", m_DeviceProps->wBitsPerSample);

		hr = FileReader::CreateFile(m_FilePath, &m_File);
		CHECK_FAILURE(hr);

		if (m_File->GetFileType() == FileType::WAV_FILE) m_Position = WAV_HEADER_SIZE;
	}
}