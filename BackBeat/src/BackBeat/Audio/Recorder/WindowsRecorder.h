#pragma once

#include "BackBeat/Core/UUID.h"
#include "Platform/Windows/Windows.h"
#include "DeviceRecorder.h"
namespace BackBeat {

	class WindowsRecorder : public DeviceRecorder
	{
	public:
		WindowsRecorder();
		~WindowsRecorder();

		// Recorder functions
		virtual void Start() override;
		virtual void Stop() override;
		virtual void Reset() override; 
		virtual void Reset(AudioProps props) override;
		virtual bool SaveWAV(std::string filePath) override;
		virtual std::shared_ptr<Track> GetRecordingTrack() override;
		
		inline virtual bool IsRecording() { return m_IsRecording; }
		inline virtual TimeMinSec GetLengthMinSec() override { return m_Recording.GetLengthMinSecs(); }
		inline virtual RecorderType GetType() override { return RecorderType::windows; }
		inline virtual UUID GetID() override { return m_ID; }

		// DeviceRecorder functions
		inline virtual void SetRecordingTrack(std::shared_ptr<Track> track) override { m_Recording.SetTrack(track); }
		inline virtual void ClearTrack() override { m_Recording.ClearTrack(); }
		inline virtual AudioProps GetProps() override { return m_AudioProps; }

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

	private:
		void Record();
		void ReleaseAll();
		void InitWindowsClient();
	};

}