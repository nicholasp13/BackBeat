#pragma once

#include <mmdeviceapi.h>
#include <Audioclient.h>

#include "Platform/Windows/Windows.h"
#include "BackBeat/Audio/Audio.h"
#include "BackBeat/Audio/Renderer/Renderer.h"
namespace BackBeat {

	class WindowsRenderer : public Renderer
	{
	public:
		WindowsRenderer();
		~WindowsRenderer();

		void Init();

		virtual void Start() override;
		virtual void Stop() override;

		inline virtual bool IsRendering() override { return m_Rendering; }
		inline virtual Mixer* GetMixer() override { return m_Mixer; };

		inline unsigned int GetBufferSize() { return m_BufferSize; }
		inline AudioProps GetProps() { return m_Props; }
		inline void SetMixer(Mixer* mixer) { m_Mixer = mixer; }

	private:
		bool m_Rendering;
		bool m_Init;
		UINT32 m_BufferSize;  // In frames (num channels not included)
		std::thread m_Thread;
		AudioProps m_Props;
		Mixer* m_Mixer;

		// Windows API members
		REFERENCE_TIME m_ActualBufferDuration;
		IAudioClient* m_AudioClient;
		IMMDeviceEnumerator* m_Enumerator;
		IMMDevice* m_Device;
		IAudioRenderClient* m_ClientRenderer;
		tWAVEFORMATEX* m_DeviceProps;

	private:
		void Render();
		void ReleaseAll();
	};
}