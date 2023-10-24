#pragma once

#include <mmdeviceapi.h>
#include <Audioclient.h>

#include "Renderer.h"
#include "BackBeat/Audio/Audio.h"
#include "RenderInfo.h"
#include "BackBeat/Audio/Synth/AudioEngine/AudioEngine.h"
namespace BackBeat {

	class WindowsRenderer : public Renderer
	{
	public:
		WindowsRenderer(tWAVEFORMATEX props, std::shared_ptr<RenderInfo> info, std::shared_ptr<AudioEngine> engine);
		~WindowsRenderer();

		virtual void Render();

		virtual void Stop() { m_Rendering = false; }
		virtual bool IsRendering() { return m_Rendering; }

	private:
		bool m_Rendering;
		UINT32 m_BufferSize;
		REFERENCE_TIME m_ActualBufferDuration;

		std::shared_ptr<RenderInfo> m_Info;
		std::shared_ptr<AudioEngine> m_Engine;

		IAudioClient* m_AudioClient;
		IMMDeviceEnumerator* m_Enumerator;
		IMMDevice* m_Device;
		IAudioRenderClient* m_ClientRenderer;
		tWAVEFORMATEX* m_DeviceProps;
		tWAVEFORMATEX m_SynthProps;

		void InitRenderer();
		void GetData(BYTE* data, UINT32 framesAvaiable);
	};
}