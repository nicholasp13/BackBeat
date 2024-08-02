#pragma once

#include "Recording.h"
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
		virtual void Start() {}
		virtual void Stop() {}
		virtual void Reset() {}
		virtual void Reset(AudioProps props) {}
		virtual void Record(char* data, unsigned int numSamples) {}
		virtual bool SaveWAV(std::string filePath) { return false; }
		virtual bool IsRecording() { return false; }
		virtual TimeMinSec GetLengthMinSec() { return TimeMinSec(); }
		virtual RecorderType GetType() { return RecorderType::none; }

	};
}