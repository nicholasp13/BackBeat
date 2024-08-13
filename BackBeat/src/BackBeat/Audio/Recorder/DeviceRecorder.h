#pragma once

#include "Recorder.h"
namespace BackBeat {

	class DeviceRecorder : public Recorder
	{
	public:
		virtual AudioProps GetProps() { return AudioProps(); }
		virtual void SetRecordingTrack(std::shared_ptr<Track> track) {}
		virtual void ClearTrack() {}

	};

}