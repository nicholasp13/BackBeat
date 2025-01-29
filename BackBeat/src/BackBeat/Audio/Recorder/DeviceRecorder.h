#pragma once

// NOTE: No current functions are different this is just in case of future uses

#include "Recorder.h"
namespace BackBeat {

	class DeviceRecorder : public Recorder
	{
	public:
		// Recorder functions
		virtual void Start() override {}
		virtual void Stop() override {}
		virtual void Reset() override {}
		virtual void Reset(AudioProps props) override {}
		virtual bool SaveWAV(std::string filePath) override { return false; }
		virtual void ClearTrack() override {}
		virtual bool IsRecording() override { return false; }
		virtual AudioProps GetProps() override { return AudioProps(); }
		virtual TimeMinSec GetLengthMinSec() override { return TimeMinSec(); }
		virtual RecorderType GetType() override { return RecorderType::device; }
		virtual std::shared_ptr<MappedTrack> GetRecordingTrack() override { return nullptr; }
		virtual UUID GetID() override { return UUID(0); }
		virtual void SetRecording(std::shared_ptr<Recording> recording) {}

	};


}