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
		virtual HRESULT FormatStream(tWAVEFORMATEX* deviceProps);
		virtual HRESULT LoadBuffer(UINT32 framesAvailable, BYTE* buffer, DWORD* flag);
		virtual FileType GetFileType();
	};
}