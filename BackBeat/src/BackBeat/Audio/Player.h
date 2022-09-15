#pragma once

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

	private:
		IAudioClient* m_AudioClient;
		IMMDeviceEnumerator* m_Enumerator;
		IMMDevice* device;
	};
}