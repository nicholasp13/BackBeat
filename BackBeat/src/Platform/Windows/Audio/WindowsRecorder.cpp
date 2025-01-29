#include "bbpch.h"

/**
 *  Windows API code: https://learn.microsoft.com/en-us/windows/win32/coreaudio/capturing-a-stream
/**/

#include "WindowsRecorder.h"  
namespace BackBeat {

	WindowsRecorder::WindowsRecorder()
		: 
		m_IsRecording(false), 
		m_Init(false),
		m_Index(0),
		m_Props(AudioProps()), 
		m_Recording(nullptr),
		m_BufferSize(0),
		m_ActualBufferDuration(0),
		m_Enumerator(NULL),
		m_Device(NULL),
		m_AudioClient(NULL),
		m_CaptureClient(NULL),
		m_DeviceProps(NULL)
	{
		
	}

	WindowsRecorder::~WindowsRecorder()
	{
		Stop();
		ReleaseAll();
	}

	void WindowsRecorder::Start()
	{
		if (!m_Init)
			return;

		m_Recording->GetTrack()->SetWritePosition(0);
		m_IsRecording = true;
		Record();
	}

	void WindowsRecorder::Stop()
	{
		m_IsRecording = false;
	}

	void WindowsRecorder::Reset()
	{
		if (!m_IsRecording)
			m_Recording->Reset();
	}
	void WindowsRecorder::Reset(AudioProps props)
	{
		if (!m_IsRecording)
			m_Recording->Reset(props);
	}

	bool WindowsRecorder::SaveWAV(std::string filePath)
	{
		if (!m_IsRecording)
			return m_Recording->SaveWAV(filePath);
		return false;
	}

	std::shared_ptr<MappedTrack> WindowsRecorder::GetRecordingTrack()
	{
		if (m_IsRecording)
			return nullptr;
		return m_Recording->GetTrack();
	}

	void WindowsRecorder::Init()
	{
		if (m_Init)
			return;

		HRESULT hr = S_OK;
		REFERENCE_TIME requestedDuration = Windows::ReftimesPerMillisecond;

		hr = CoCreateInstance(
			Windows::CLSID_MMDeviceEnumerator,
			NULL,
			CLSCTX_ALL,
			Windows::IID_IMMDeviceEnumerator,
			(void**)&m_Enumerator);
		CHECK_FAILURE_HRESULT(hr);

		hr = m_Enumerator->GetDefaultAudioEndpoint(eCapture, eConsole, &m_Device);
		CHECK_FAILURE_HRESULT(hr);

		hr = m_Device->Activate(Windows::IID_IAudioClient, CLSCTX_ALL, NULL, (void**)&m_AudioClient);
		CHECK_FAILURE_HRESULT(hr);

		hr = m_AudioClient->GetMixFormat(&m_DeviceProps);
		CHECK_FAILURE_HRESULT(hr);

		hr = m_AudioClient->Initialize(
			AUDCLNT_SHAREMODE_SHARED,
			0,
			requestedDuration,
			0,
			m_DeviceProps,
			NULL);
		CHECK_FAILURE_HRESULT(hr);

		hr = m_AudioClient->GetBufferSize(&m_BufferSize);
		CHECK_FAILURE_HRESULT(hr);

		hr = m_AudioClient->GetService(Windows::IID_IAudioCaptureClient, (void**)&m_CaptureClient);
		CHECK_FAILURE_HRESULT(hr);

		// Sets other members after Windows members are successfuly initialized
		m_Init = true;
		m_ActualBufferDuration = (REFERENCE_TIME)Windows::ReftimesPerSecond * m_BufferSize
			/ m_DeviceProps->nSamplesPerSec;
		m_Props.bigEndian = Audio::IsBigEndian(); // NOTE: This assumes that Windows API keeps the endianness the same as the user
		Windows::AudioPropsConversion(m_DeviceProps, &m_Props);
		m_Props.format = Audio::GetAudioFormat(m_Props.bitDepth, false);
		
		if (m_Recording)
			m_Recording->Reset(m_Props);

		return;

	FailureExit:
		BB_CORE_ERROR("Windows Capture client was not set");
		ReleaseAll();
	}

	void WindowsRecorder::Record()
	{
		HRESULT hr = S_OK;
		UINT32 padding = 0;
		INT32 position = 0;
		DWORD flags = 0;
		UINT32 packetLength = 0;
		UINT32 framesAvailable = 0;
		DWORD sleepTime = (DWORD)(m_ActualBufferDuration / Windows::ReftimesPerMillisecond / 2);
		BYTE* data = nullptr;

		hr = m_AudioClient->Start();
		CHECK_FAILURE_HRESULT(hr);

		while (m_IsRecording)
		{
			Sleep(sleepTime);

			hr = m_CaptureClient->GetNextPacketSize(&packetLength);
			CHECK_FAILURE_HRESULT(hr);

			while (packetLength != 0)
			{
				hr = m_CaptureClient->GetBuffer(
					&data,
					&framesAvailable,
					&flags,
					NULL,
					NULL);
				CHECK_FAILURE_HRESULT(hr);

				// Recording checks if data != nullptr, if so then it records 0s for the framesAvailable
				if (flags & AUDCLNT_BUFFERFLAGS_SILENT)
					data = nullptr;

				m_Recording->Record((char*)data, framesAvailable, m_Props);

				hr = m_CaptureClient->ReleaseBuffer(framesAvailable);
				CHECK_FAILURE_HRESULT(hr);

				hr = m_CaptureClient->GetNextPacketSize(&packetLength);
				CHECK_FAILURE_HRESULT(hr);
			}
		}
		Sleep(sleepTime);

		hr = m_AudioClient->Stop();
		CHECK_FAILURE_HRESULT(hr);

		return;

	FailureExit:
		m_IsRecording = false;
	}

	void WindowsRecorder::ReleaseAll()
	{
		m_Init = false;
		CoTaskMemFree(m_DeviceProps);
		if (m_Enumerator != NULL)
			m_Enumerator->Release();
		if (m_Device != NULL)
			m_Device->Release();
		if (m_AudioClient != NULL)
			m_AudioClient->Release();
		if (m_CaptureClient != NULL)
			m_CaptureClient->Release();
	}

}