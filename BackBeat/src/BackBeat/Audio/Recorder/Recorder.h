#pragma once

#include "Recording.h"
namespace BackBeat {

	class Recorder
	{
	public:
		virtual void Start() {}
		virtual void Stop() {}
		virtual void Reset() {}
		// NOTE: Future implementation should require requested properties that the Recording class will match
		//       when outputting the file
		virtual bool SaveWAV(std::string filePath) { return false; }
		virtual bool IsRecording() { return true; }
		// NOTE: This is where the temp recording file is NOT where the final WAV file will be saved to.
		virtual void SetTempPath(std::string filePath) {}
		virtual TimeMinSec GetLengthMinSec() { return TimeMinSec(); }
	};
}