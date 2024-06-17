#pragma once

#include <mmdeviceapi.h>
#include <Audioclient.h>

#include "BackBeat/Audio/Audio.h"
#include "Renderer.h"
namespace BackBeat {

	class WindowsRenderer : public Renderer
	{
	public:
		WindowsRenderer();
		~WindowsRenderer();

		virtual void Start();
		virtual void Stop();

		inline virtual bool IsRendering() { return m_Rendering; }
		inline virtual std::shared_ptr<Mixer> GetMixer() { return m_Mixer; };

	private:
		bool m_Rendering;
		UINT32 m_BufferSize;
		std::thread m_Thread;
		AudioProps m_Props;
		std::shared_ptr<Mixer> m_Mixer;

		// Windows members
		REFERENCE_TIME m_ActualBufferDuration;
		IAudioClient* m_AudioClient;
		IMMDeviceEnumerator* m_Enumerator;
		IMMDevice* m_Device;
		IAudioRenderClient* m_ClientRenderer;
		tWAVEFORMATEX* m_DeviceProps;

		void Render();
		void InitRenderer();
	};
}