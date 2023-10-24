#include "bbpch.h"

#include "Synth.h"
namespace BackBeat {

	Synth::Synth()
	{
		InitSynth();
	}

	Synth::~Synth()
	{

	}

	void Synth::Start()
	{
		if (!m_Running) {
			m_Running = true;
			Run();
		}
	}

	void Synth::Stop()
	{
		m_Running = false;
		m_Renderer->Stop();
		if (m_Thread.joinable())
			m_Thread.join();
	}

	void Synth::InitSynth()
	{
		m_Running = false;
		m_Props = {
			.wFormatTag =      (WORD)  0,
			.nChannels =       (WORD)  2,
			.nSamplesPerSec =  (DWORD) 48000,
			.nAvgBytesPerSec = (DWORD) 384000,
			.nBlockAlign =     (WORD)  8,
			.wBitsPerSample =  (WORD)  32, 
			.cbSize =          (WORD)  0
		};

		m_Timer = std::make_shared<Timer>();
		m_Engine = std::make_shared<AudioEngine>(m_Props);
		m_RenderInfo = std::make_shared<RenderInfo>(m_Props);
		m_Renderer = std::make_shared<WindowsRenderer>(m_Props, m_RenderInfo, m_Engine);
		m_Handler = std::make_shared<SynthEventHandler>(m_RenderInfo);
	}

	void Synth::Run()
	{
		m_Thread = std::thread(&Renderer::Render, m_Renderer);
	}

	void Synth::SyncBuffers()
	{

	}
}