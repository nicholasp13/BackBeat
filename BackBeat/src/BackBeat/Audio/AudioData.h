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
		virtual HRESULT LoadBuffer(UINT32 framesAvailable, BYTE* buffer, unsigned int* position, DWORD* flag) { return S_OK; }
		virtual FileType GetFileType() { return FileType::None; }

	};
}