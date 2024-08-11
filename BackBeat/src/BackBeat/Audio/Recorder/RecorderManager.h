#pragma once

/**
 * NOTES: This objeect keeps track of all recorders. Each recorder will be stored by a hash key of its UUID of a BBobject that
 *        the recorder is assigned to recording.
/**/

#include <unordered_map>

#include "DeviceRecorder.h"
#include "RecorderBuilder.h"
#include "BackBeat/Core/UUID.h"
namespace BackBeat {

	class RecorderManager
	{
	public:
		RecorderManager();
		~RecorderManager();

		void Start();
		void Stop();
		std::shared_ptr<Recorder> AddRecorder(UUID id, AudioProps props);
		void AddDeviceRecorder(std::shared_ptr<DeviceRecorder> recorder);
		void Record(UUID id, void* buffer, unsigned int numSamples);
		std::shared_ptr<Recorder> GetRecorder(UUID id);
		void SetRecorderActive(UUID id);
		void SetRecorderInactive(UUID id);
		void SetDeviceRecorderTrack(UUID trackID, std::shared_ptr<Track> track);
		void ClearDeviceTrack();

		inline void DeleteRecorder(UUID id) { m_Recorders.erase(id); }
		inline bool IsOn(UUID id) { return m_ActiveID == id; }
		inline bool IsDeviceTrackOn(UUID id) { return m_ActiveDeviceTrackID == id; }
		inline bool IsRecording() { return m_Recording; }
		inline bool IsDeviceRecording() { return m_DeviceRecorder->IsRecording(); } // TODO: Put in .cpp file to check if 
		inline std::shared_ptr<DeviceRecorder> GetDeviceRecorder() { return m_DeviceRecorder; }
		
	private:
		bool m_Recording;
		std::unordered_map<UUID, std::shared_ptr<Recorder>> m_Recorders;
		std::shared_ptr<DeviceRecorder> m_DeviceRecorder;
		UUID m_ActiveID;
		UUID m_ActiveDeviceTrackID;

	private:
		inline bool Contains(UUID id) { return !(m_Recorders.find(id) == m_Recorders.end()); }
	};

}