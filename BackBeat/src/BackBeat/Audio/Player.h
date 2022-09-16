#pragma once

#include "Audio.h"

#include <mmdeviceapi.h>
#include <Audioclient.h>

namespace BackBeat {

	class Player
	{
	public:
		Player();
		virtual ~Player();

		void Play();
		void Pause();
		void Stop();
		void SetFile();

		bool FileSelected = FALSE;
	private:
		UINT32 m_BufferSize;

		IAudioClient* m_AudioClient;
		IMMDeviceEnumerator* m_Enumerator;
		IMMDevice* m_Device;
		IAudioRenderClient* m_Renderer;

		AudioProps m_FileProps;
	};
}