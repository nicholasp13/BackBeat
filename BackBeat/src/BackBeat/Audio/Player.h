#pragma once

#include <string>
#include <mmdeviceapi.h>
#include <Audioclient.h>

namespace BackBeat {

	class Player
	{
	public:
		Player(std::string filePath);
		virtual ~Player();

		void Play();
		void Pause();
		void Stop();

		bool FileSelected = FALSE;
	private:
		void InitAudioClient();

		std::string m_FilePath;

		IAudioClient* m_AudioClient;
		IMMDeviceEnumerator* m_Enumerator;
		IMMDevice* m_Device;
		IAudioRenderClient* m_Renderer;
		tWAVEFORMATEX* m_DeviceProps;
		tWAVEFORMATEX* m_FileProps;
	};
}