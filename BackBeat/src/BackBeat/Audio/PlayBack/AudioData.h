#pragma once

#include "bbpch.h"

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
		virtual bool LoadBuffer(unsigned int framesAvailable, byte* buffer, unsigned int* position, bool* loading) { return false; }
		
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