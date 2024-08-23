#pragma once

#include <mmdeviceapi.h>
#include <Audioclient.h>

#include "Platform/Windows/Windows.h"
#include "BackBeat/Audio/Audio.h"
#include "BackBeat/Audio/Renderer/Renderer.h"
// TO DELETE
#include "BackBeat/Core/Timer.h"
namespace BackBeat {

	class WindowsRenderer : public Renderer
	{
	public:
		WindowsRenderer();
		~WindowsRenderer();

		void Init();

		virtual void Start() override;
		virtual void Stop() override;
		virtual void Render() override;
		virtual void RenderFree() override;

		inline virtual bool IsRendering() override { return m_Rendering; }
		inline virtual Mixer* GetMixer() override { return m_Mixer; };
		inline virtual void SetMixer(Mixer* mixer) override { m_Mixer = mixer; }

		inline unsigned int GetBufferSize() { return m_BufferSize; }
		inline AudioProps GetProps() { return m_Props; }

	private:
		bool m_Rendering;
		bool m_Init;
		UINT32 m_BufferSize;  // In frames (num channels not included)
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
		void ReleaseAll();
	};
}