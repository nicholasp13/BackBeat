#pragma once

/**
 * NOTES: This objeect keeps track of all recorders. Each recorder will be stored by a hash key of its UUID of a BBobject that
 *        the recorder is assigned to recording.
/**/

#include <unordered_map>

#include "BackBeat/Core/UUID.h"
#include "BackBeat/Audio/PlayBack/TrackFactory.h"
#include "BackBeat/Audio/Helpers/AudioThread.h"
#include "BackBeat/Core/Timer.h"
#include "DeviceRecorder.h"
namespace BackBeat {

	class RecorderManager
	{
	public:
		RecorderManager();
		~RecorderManager();

		void Init(Recorder* recorder, DeviceRecorder* deviceRecorder);
		void Start();
		void Stop();
		std::shared_ptr<MappedTrack> AddRecordingMappedTrack(UUID id, RecorderType type);
		void SetRecorderActive(UUID id);
		void SetRecorderInactive(UUID id);
		void SetRecordingTrack(UUID id);
		void SetDeviceRecorderIndex(UUID id, unsigned int index);
		void ClearTrack(UUID id);
		void DeleteTrack(UUID id);
		void ResetRecording(UUID id);
		void ResetRecording(UUID id, AudioProps props);

		TimeMinSec GetTime();
		TimeMinSec GetTimeSeconds();
		TimeMinSec GetTimeMs();
		
		inline bool IsActive(UUID id) { return m_ActiveID == id; }
		inline bool IsRecording() { return m_Recording; }
		inline Recorder* GetAudioRecorder() { return m_AudioRecorder; }
		inline Recorder* GetDeviceRecorder() { return m_DeviceRecorder; }
		
	private:
		bool m_Recording;
		bool m_Init;
		float m_TimeEclipsed;

		Timer m_Timer;
		AudioThread m_Thread;
		Recorder* m_AudioRecorder;
		DeviceRecorder* m_DeviceRecorder;
		UUID m_ActiveID;
		std::unordered_map<UUID, std::shared_ptr<Recording>> m_AudioRecordings;
		std::unordered_map<UUID, std::shared_ptr<Recording>> m_DeviceRecordings;

	private:
		inline bool Contains(UUID id) { return (ContainsAudio(id) || ContainsDevice(id)); }
		inline bool ContainsAudio(UUID id) { return m_AudioRecordings.find(id) != m_AudioRecordings.end(); }
		inline bool ContainsDevice(UUID id) { return m_DeviceRecordings.find(id) != m_DeviceRecordings.end(); }

	};

}