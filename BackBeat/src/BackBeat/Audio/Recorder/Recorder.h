#pragma once

#include "Recording.h"
#include "BackBeat/Core/UUID.h"
namespace BackBeat {

	enum class RecorderType
	{
		none = 0,
		audio,
		device
	};

	class Recorder
	{
	public:
		virtual void Start() {}
		virtual void Stop() {}
		virtual void Reset() {}
		virtual void Reset(AudioProps props) {}
		virtual bool SaveWAV(std::string filePath) { return false; }
		virtual void ClearTrack() {}
		virtual bool IsRecording() { return false; }
		virtual AudioProps GetProps() { return AudioProps(); }
		virtual TimeMinSec GetLengthMinSec() { return TimeMinSec(); }
		virtual RecorderType GetType() { return RecorderType::none; }
		virtual std::shared_ptr<MappedTrack> GetRecordingTrack() { return nullptr; }
		virtual UUID GetID() { return UUID(0); }
		virtual void SetRecording(std::shared_ptr<Recording> recording) {}

	};
}