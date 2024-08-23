#include "bbpch.h"

#include "AudioEngine.h"
namespace BackBeat {

	AudioEngine::AudioEngine()
		: 
		m_Init(false), 
		m_Free(false),
		m_Props(AudioProps()),
		m_Renderer(nullptr)
	{

	}

	AudioEngine::~AudioEngine()
	{

	}

	void AudioEngine::Init(AudioProps props, Renderer* renderer, RecorderManager* recordingMgr, bool free)
	{
		m_Props = props;
		m_Renderer = renderer;
		m_Free = free;

		m_RecordingSink.Init(m_Props);
		m_VisualizingSink.Init(m_Props);
		m_Mixer.Init(m_Props, &m_RecordingSink, &m_VisualizingSink);
		m_Mixer.SetRecordingManager(recordingMgr);
		m_Mixer.SetVisualizer(&m_Visualizer);
		m_Renderer->SetMixer(&m_Mixer);

		m_Visualizer.Init(m_Props, &m_VisualizingSink, m_Props.sampleRate * m_Props.numChannels / 10);

		m_Init = true;
	}

	void AudioEngine::Callback()
	{
		if (!m_Init)
			return;
		
		if (m_Free)
		{
			m_Renderer->Start();
			m_Renderer->RenderFree();
		}
		else
		{
			m_Renderer->Render();
		}
	}

}