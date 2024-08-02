#pragma once

/**
 * NOTES: This objeect keeps track of all recorders. Each recorder will be stored by a hash key of its UUID of a BBobject that
 *        the recorder is assigned to recording.
/**/

#include <unordered_map>

#include "Recorder.h"
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
		void AddRecorder(UUID id, AudioProps props);
		void Record(UUID id, void* buffer, unsigned int numSamples);
		std::shared_ptr<Recorder> GetRecorder(UUID id);

		inline bool IsRecording() { return m_Recording; }

	private:
		bool m_Recording;
		std::unordered_map<UUID, std::shared_ptr<Recorder>> m_Recorders;
	};

}