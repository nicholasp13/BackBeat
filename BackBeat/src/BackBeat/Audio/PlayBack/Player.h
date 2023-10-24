#pragma once

/*
* Audio player that uses Windows API.
* Only 48K sample and stereo .WAV files
*/

// ------------------------------------------------------------------------------- //
// TODO:
// - Rename and refactor into a WindowsPlayer and an implementation of a Player
//       abstract class for clarification on platform and for possible 
//       implementation of Linux player.
// - Allow for other file types i.e. MP3s, other sample rates, other bit depth,
//       and 
// - Create an exclusive Windows audio mode that only allows for Audio from this
//      application to play
// ------------------------------------------------------------------------------- //

#include <string>
#include <mmdeviceapi.h>
#include <Audioclient.h>

#include "BackBeat/Audio/Audio.h"
#include "AudioData.h"
#include "Loader.h"
namespace BackBeat {

	class Player
	{
	public:
		
		Player();
		~Player();

		void Play();
		void PlaySamples(UINT32 samples);
		void Pause();
		void Stop();

		tWAVEFORMATEX* GetProps() { return m_DeviceProps; }
		void SetLoader(std::shared_ptr<Loader> loader) { m_Loader = loader; }

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

		std::shared_ptr<AudioData> m_File;
		std::shared_ptr<Loader> m_Loader;
	};
}