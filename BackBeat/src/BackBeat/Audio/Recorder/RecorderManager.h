#pragma once

/**
 * NOTES: This objeect keeps track of all recorders. Each recorder will be stored by a hash key of its UUID of a BBobject that
 *        the recorder is assigned to recording.
/**/

#include <unordered_map>

#include "BackBeat/Core/UUID.h"
#include "BackBeat/Audio/PlayBack/TrackFactory.h"
#include "BackBeat/Audio/Helpers/AudioThread.h"
#include "Recorder.h"
namespace BackBeat {

	class RecorderManager
	{
	public:
		RecorderManager();
		~RecorderManager();

		void Init(Recorder* recorder, Recorder* deviceRecorder);
		void Start();
		void Stop();
		std::shared_ptr<Track> AddRecordingTrack(UUID id, RecorderType type);
		void AddRecordingTrack(UUID id, std::shared_ptr<Track> track, RecorderType type);
		void SetRecorderActive(UUID id);
		void SetRecorderInactive(UUID id);
		void SetRecordingTrack(UUID id);
		void ClearTrack(UUID id);
		void DeleteTrack(UUID id);
		void ResetRecording(UUID id);
		
		inline bool IsActive(UUID id) { return m_ActiveID == id; }
		inline bool IsRecording() { return m_Recording; }
		inline Recorder* GetAudioRecorder() { return m_AudioRecorder; }
		inline Recorder* GetDeviceRecorder() { return m_DeviceRecorder; }
		
	private:
		bool m_Recording;
		bool m_Init;
		AudioThread m_Thread;
		Recorder* m_AudioRecorder;
		Recorder* m_DeviceRecorder;
		UUID m_ActiveID;
		std::unordered_map<UUID, std::shared_ptr<Track>> m_AudioRecordings;
		std::unordered_map<UUID, std::shared_ptr<Track>> m_DeviceRecordings;

	private:
		inline bool Contains(UUID id) { return (ContainsAudio(id) || ContainsDevice(id)); }
		inline bool ContainsAudio(UUID id) { return m_AudioRecordings.find(id) != m_AudioRecordings.end(); }
		inline bool ContainsDevice(UUID id) { return m_DeviceRecordings.find(id) != m_DeviceRecordings.end(); }

	};

}