#pragma once

#include "bbpch.h"

#include <mmeapi.h>

namespace BackBeat {

	enum class FileType
	{
		None = 0,
		WAV_FILE,
		MP3_FILE,
		VOICE // SUBJECT TO CHANGE WITH MORE IMPLEMENTATION
	};

	class AudioData
	{
	public:
		virtual HRESULT LoadBuffer(UINT32 framesAvailable, BYTE* buffer, UINT32* position, bool* loading) { return S_OK; }
		virtual FileType GetFileType() { return FileType::None; }
		virtual tWAVEFORMATEX* GetProperties() { return NULL; }
		virtual UINT32 GetSize() { return 0;  }
	};
}