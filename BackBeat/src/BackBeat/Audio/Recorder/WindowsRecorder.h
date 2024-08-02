#pragma once

#include "BackBeat/Core/UUID.h"
#include "Platform/Windows/Windows.h"
#include "Recorder.h"
namespace BackBeat {

	class WindowsRecorder : public Recorder
	{
	public:
		WindowsRecorder();
		~WindowsRecorder();

		virtual void Start();
		virtual void Stop();
		virtual void Reset(); 
		virtual void Reset(AudioProps props);
		virtual bool SaveWAV(std::string filePath);
		
		inline virtual TimeMinSec GetLengthMinSec() { return m_Recording.GetLengthMinSecs(); }
		inline virtual RecorderType GetType() { return RecorderType::windows; }
		inline virtual bool IsRecording() { return m_IsRecording; }

	private:
		bool m_IsRecording;
		std::thread m_Thread;
		UUID m_ID;
		AudioProps m_AudioProps;
		Recording m_Recording;

		// Windows API members
		UINT32 m_BufferSize;
		REFERENCE_TIME m_ActualBufferDuration;
		IMMDeviceEnumerator* m_Enumerator;
		IMMDevice* m_Device;
		IAudioClient* m_AudioClient;
		IAudioCaptureClient* m_CaptureClient;
		WAVEFORMATEX* m_DeviceProps;

		void Record();
		void ReleaseAll();
		void InitWindowsClient();
	};

}