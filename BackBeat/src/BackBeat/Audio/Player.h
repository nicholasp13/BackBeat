#pragma once

#include <string>
#include <mmdeviceapi.h>
#include <Audioclient.h>

#include "Audio.h"
#include "AudioData.h"
#include "FileReader.h"
#include "Loader.h"

namespace BackBeat {

	class Player
	{
	public:
		
		Player();
		virtual ~Player();

		void Play();
		void PlaySamples(UINT32 samples);
		void Pause();
		void Stop();

		tWAVEFORMATEX* GetProps() { return m_DeviceProps; }
		void SetLoader(Loader* loader) { m_Loader = loader; }

		bool Playing = false;

	private:
		void InitAudioClient();

		UINT32 m_Position;
		UINT32 m_BufferSize;
		REFERENCE_TIME m_ActualBufferDuration;
		std::thread m_Worker;

		IAudioClient* m_AudioClient;
		IMMDeviceEnumerator* m_Enumerator;
		IMMDevice* m_Device;
		IAudioRenderClient* m_Renderer;
		tWAVEFORMATEX* m_DeviceProps;

		AudioData* m_File;
		Loader* m_Loader;
	};
}