#pragma once

#include "bbpch.h"

#include <mmeapi.h>

namespace BackBeat {

	enum class FileType
	{
		None = 0,
		WAV,
		MP3
	};

	class AudioData
	{
	public:
		virtual HRESULT LoadBuffer(UINT32 framesAvailable, BYTE* buffer, UINT32* position) { return S_OK; }
		virtual FileType GetFileType() { return FileType::None; }
		virtual tWAVEFORMATEX* GetProperties() { return NULL;  }
	};
}