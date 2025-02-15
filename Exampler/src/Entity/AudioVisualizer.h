#pragma once

#include <BackBeat.h>
namespace Exampler {

	class AudioVisualizer 
	{
	public:
		AudioVisualizer(float height = 400.0f, float width = 800.0f, unsigned int bufferSize = 0, BackBeat::Visualizer * visualizer = nullptr);
		AudioVisualizer(unsigned int bufferSize = 0, BackBeat::Visualizer* visualizer = nullptr);
		~AudioVisualizer();

		AudioVisualizer(const AudioVisualizer& other);
		AudioVisualizer(AudioVisualizer&& rhs) noexcept;

		AudioVisualizer& operator = (const AudioVisualizer& rhs) noexcept;
		AudioVisualizer& operator = (AudioVisualizer&& rhs) noexcept;

		void Update();
		void Render();

		inline void Open() { m_Open = true; }
		inline void Close() { m_Open = false; }

	private:
		bool m_Open;
		float m_Width;
		float m_Height;
		unsigned int m_BufferSize;
		float* m_Buffer;

		BackBeat::Spectrogram m_Spectrogram;
		BackBeat::Visualizer* m_Visualizer;

	private:
		void RenderChannels();
		void RenderSpectrogram();
	};

}