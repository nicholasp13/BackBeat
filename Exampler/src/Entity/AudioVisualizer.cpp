#include "AudioVisualizer.h"

namespace Exampler {

	AudioVisualizer::AudioVisualizer(float height, float width, unsigned int bufferSize, BackBeat::Visualizer* visualizer)
		:
		m_Open(false),
		m_Height(height),
		m_Width(width),
		m_BufferSize(bufferSize),
		m_Buffer(new float[bufferSize]),
		m_Spectrogram(bufferSize),
		m_Visualizer(visualizer)
	{

	}

	AudioVisualizer::AudioVisualizer(unsigned int bufferSize, BackBeat::Visualizer* visualizer)
		:
		m_Open(false),
		m_Height(400.0f),
		m_Width(800.0f),
		m_BufferSize(bufferSize),
		m_Buffer(new float[bufferSize]),
		m_Spectrogram(bufferSize),
		m_Visualizer(visualizer)
	{

	}

	AudioVisualizer::~AudioVisualizer()
	{
		delete[m_BufferSize] m_Buffer;
	}

	AudioVisualizer::AudioVisualizer(const AudioVisualizer& other)
	{
		delete m_Buffer;

		m_Open = other.m_Open;
		m_Height = other.m_Height;
		m_Width = other.m_Width;
		m_BufferSize = other.m_BufferSize;
		m_Buffer = new float[m_BufferSize];
		m_Spectrogram = other.m_Spectrogram;
		m_Visualizer = other.m_Visualizer;

		memcpy(m_Buffer, other.m_Buffer, std::size_t(m_BufferSize * sizeof(float)));
	}

	AudioVisualizer::AudioVisualizer(AudioVisualizer&& rhs) noexcept
	{
		delete m_Buffer;

		m_Open = rhs.m_Open;
		m_Height = rhs.m_Height;
		m_Width = rhs.m_Width;
		m_BufferSize = rhs.m_BufferSize;
		m_Buffer = rhs.m_Buffer;
		m_Spectrogram = rhs.m_Spectrogram;
		m_Visualizer = rhs.m_Visualizer;

		rhs.m_Buffer = nullptr;
	}

	AudioVisualizer& AudioVisualizer::operator = (const AudioVisualizer& rhs) noexcept
	{
		delete m_Buffer;

		m_Open = rhs.m_Open;
		m_Height = rhs.m_Height;
		m_Width = rhs.m_Width;
		m_BufferSize = rhs.m_BufferSize;
		m_Buffer = new float[m_BufferSize];
		m_Spectrogram = rhs.m_Spectrogram;
		m_Visualizer = rhs.m_Visualizer;

		memcpy(m_Buffer, rhs.m_Buffer, std::size_t(m_BufferSize * sizeof(float)));

		return *this;
	}

	AudioVisualizer& AudioVisualizer::operator = (AudioVisualizer&& rhs) noexcept
	{
		delete m_Buffer;

		m_Open = rhs.m_Open;
		m_Height = rhs.m_Height;
		m_Width = rhs.m_Width;
		m_BufferSize = rhs.m_BufferSize;
		m_Buffer = rhs.m_Buffer;
		m_Spectrogram = rhs.m_Spectrogram;
		m_Visualizer = rhs.m_Visualizer;

		rhs.m_Buffer = nullptr;

		return *this;
	}

	void AudioVisualizer::AudioVisualizer::Update()
	{
		if (m_Visualizer)
			m_Visualizer->Update();
	}

	void AudioVisualizer::AudioVisualizer::Render()
	{
		if (!m_Open)
			return;

		ImGui::SetNextWindowSize(ImVec2(m_Width, m_Height));

		ImGuiWindowFlags windowFlags = 0;
		windowFlags |= ImGuiWindowFlags_NoCollapse;
		windowFlags |= ImGuiWindowFlags_NoResize;

		ImGui::Begin("Audio Visualizer", &m_Open, windowFlags);
		ImGui::PushID("AudioVisualizer");

		RenderChannels();
		RenderSpectrogram();

		ImGui::PopID();
		ImGui::End();
	}

	void AudioVisualizer::RenderChannels()
	{
		if (!m_Visualizer->IsOn())
		{
			if (ImGui::Button("On", ImVec2(100, 20)))
				m_Visualizer->On();
		}
		else
		{
			if (ImGui::Button("Off", ImVec2(100, 20)))
				m_Visualizer->Off();
		}

		const float visualMax = 1.0f;
		auto bufferSize = m_Visualizer->GetChannelBufferSize();
		for (unsigned int i = 0; i < m_Visualizer->GetNumChannels(); i++)
		{
			ImGui::Spacing();
			std::string name = "Channel " + std::to_string(i + 1);
			ImGui::PlotLines(name.c_str(), m_Visualizer->GetChannelBuffer(i), bufferSize, 1, "",
				visualMax * -1, visualMax, ImVec2(m_Width, 60.0f));
		}

		ImGui::Spacing();
	}

	void AudioVisualizer::RenderSpectrogram()
	{
		float visualMax = 10.0f;
		auto bufferSize = m_Visualizer->GetChannelBufferSize();

		BackBeat::Audio::FlushBuffer((byte*)m_Buffer, m_BufferSize * BackBeat::Audio::FloatByteSize);

		m_Spectrogram.Get(m_Visualizer->GetChannelBuffer(0), m_Buffer, bufferSize);

		ImGui::PlotLines("Spectogram", m_Buffer, bufferSize / 2, 1, "",
			0.0f, visualMax, ImVec2(m_Width, 120.0f));

	}

}