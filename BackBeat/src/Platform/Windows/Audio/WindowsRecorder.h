#pragma once

#include "BackBeat/Core/UUID.h"
#include "Platform/Windows/Windows.h"
#include "BackBeat/Audio/Recorder/DeviceRecorder.h"
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
		virtual std::shared_ptr<MappedTrack> GetRecordingTrack() override;
		
		inline virtual void ClearTrack() override { m_Recording->ClearTrack(); }
		inline virtual bool IsRecording() { return m_IsRecording; }
		inline virtual AudioProps GetProps() override { return m_Props; }
		inline virtual TimeMinSec GetLengthMinSec() override { return m_Recording->GetLengthMinSecs(); }
		inline virtual RecorderType GetType() override { return RecorderType::device; }
		inline virtual UUID GetID() override { return m_ID; }
		inline virtual void SetRecording(std::shared_ptr<Recording> recording) override { m_Recording = recording; }

		// WindowsRecorder functions
		void Init();

	private:
		bool m_IsRecording;
		bool m_Init;
		unsigned int m_Index;
		UUID m_ID;
		AudioProps m_Props;
		std::shared_ptr<Recording> m_Recording;

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
	};

}