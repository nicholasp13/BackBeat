#pragma once

#include "bbpch.h"

// NOTE: With current implementation this could just be a struct

#include "BackBeat/Audio/Audio.h"
namespace BackBeat {

	enum class FileType
	{
		None = 0,
		WAV_FILE,
		MP3_FILE
	};

	class AudioData
	{
	public:
		virtual FileType GetFileType() { return FileType::None; }
		virtual std::string GetName() { return ""; }
		virtual std::string GetFilePath() { return ""; }
		virtual AudioProps GetProps() { return AudioProps(); }
		virtual unsigned int GetSize() { return 0; }
		virtual unsigned int GetZero() { return 0; }
		virtual unsigned int GetDataSize() { return 0; }

		virtual void SetZero(unsigned int position) {}
		virtual void SetDataSize(unsigned int size) {}
	};
}