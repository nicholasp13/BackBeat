#pragma once

#include "Recording.h"
#include "BackBeat/Core/UUID.h"
namespace BackBeat {

	enum class RecorderType
	{
		none = 0,
		audio,
		windows
	};

	class Recorder
	{
	public:
		// On/Off for Recording BBObjects
		virtual void On() {}
		virtual void Off() {}
		// Start/Stop for Device recording using system API
		virtual void Start() {}
		virtual void Stop() {}

		virtual void Reset() {}
		virtual void Reset(AudioProps props) {}
		virtual void Record(char* data, unsigned int numSamples) {}
		virtual bool SaveWAV(std::string filePath) { return false; }
		virtual bool IsOn() { return false; }
		virtual bool IsRecording() { return false; }
		virtual TimeMinSec GetLengthMinSec() { return TimeMinSec(); }
		virtual RecorderType GetType() { return RecorderType::none; }
		virtual std::shared_ptr<Track> GetRecordingTrack() { return nullptr; }
		virtual UUID GetID() { return UUID(0); }

	};
}