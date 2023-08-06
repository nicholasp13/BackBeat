#pragma once

#include <string>
#include <mmdeviceapi.h>
#include <Audioclient.h>

#include "AudioData.h"

namespace BackBeat {

	class Player
	{
	public:
		Player(std::string filePath);
		virtual ~Player();

		void Play();
		void Pause();
		void Stop();

		bool Playing = false;
	private:
		void InitAudioClient();

		std::string m_FilePath;
		UINT32 m_Position;
		UINT32 m_BufferSize;
		REFERENCE_TIME m_ActualBufferDuration;

		IAudioClient* m_AudioClient;
		IMMDeviceEnumerator* m_Enumerator;
		IMMDevice* m_Device;
		IAudioRenderClient* m_Renderer;
		tWAVEFORMATEX* m_DeviceProps;

		AudioData* m_File;
	};
}