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
// - Change implementation after checking performance (specifically Loader and how
//       the file is loaded and read)
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

	class WindowsPlayer
	{
	public:
		
		WindowsPlayer();
		~WindowsPlayer();

		void Play();
		void Pause();
		void Stop();

		tWAVEFORMATEX* GetProps() { return m_WDeviceProps; }
		void SetLoader(std::shared_ptr<Loader> loader) { m_Loader = loader; }

		bool Playing = false; // TODO: Change to private variable and change check to a function

	private:
		UINT32 m_Position;
		UINT32 m_BufferSize;
		AudioProps m_Props;
		std::thread m_Worker;

		// Windows Audio Renderer members
		REFERENCE_TIME m_ActualBufferDuration;
		IAudioClient* m_AudioClient;
		IMMDeviceEnumerator* m_Enumerator;
		IMMDevice* m_Device;
		IAudioRenderClient* m_Renderer;
		// Windows struct for audio file properties
		tWAVEFORMATEX* m_WDeviceProps;

		std::shared_ptr<AudioData> m_File;
		std::shared_ptr<Loader> m_Loader;

		void InitAudioClient();
	};
}