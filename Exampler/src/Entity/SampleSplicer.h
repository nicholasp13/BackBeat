#pragma once

#include <BackBeat.h>
#include <imgui.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>
#include <GLFW/glfw3.h>
#include <imgui-knobs.h>

namespace Exampler {

	class SampleSplicer
	{
	public:
		SampleSplicer();
		~SampleSplicer();

		void Update();
		void Render();
		void Add(BackBeat::Mixer* mixer);
		void Delete(BackBeat::Mixer* mixer);
		void Close();

		inline void Open() { m_Open = true; }
		inline bool IsOpen() { return m_Open; }

	private:
		const unsigned long BufferSize = 480000; // This is about 10 secondsd for 1 channel of a stereo 48k sample rate
		                                         // floating point AudioProps
		const float m_Width = 900.0f;
		const float m_Height = 480.0f;
		const float m_DummyHeight = 5.0f;

		bool m_Open = false;
		bool m_RenderTrack = true;
		bool m_LoopingTrack = false;
		bool m_LoopingSample = false;

		int m_Zero = 0;
		int m_Start = 0;
		int m_End = 0;
		int m_Size = 0;
		int m_StartMs = 0;
		int m_EndMs = 0;
		int m_TimePercent = 0;
		float m_Volume = 1.0f;
		std::shared_ptr<float[]> m_Buffer = std::make_shared<float[]>(BufferSize);

		BackBeat::AudioProps m_Props = BackBeat::AudioProps();
		BackBeat::SplicerParameters m_Params = BackBeat::SplicerParameters();
		BackBeat::AudioFileLoader m_Loader = BackBeat::AudioFileLoader(BufferSize * BackBeat::Audio::FloatByteSize * BackBeat::Audio::Stereo);
		BackBeat::Player m_TrackPlayer;
		BackBeat::Splicer m_Splicer;
		std::shared_ptr<BackBeat::MappedTrack> m_Track = nullptr;

	private:
		void RenderMenuBar();
		void RenderTrackControls();
		void RenderSplicerControls();
		// Track Controls render functions
		void RenderTrack();
		void RenderTrackButtons();
		void RenderTrackStartEndControls();
		// Splicer Controls render functions
		void RenderSplicerBuffers();
		void RenderButtons();
		void RenderGainControls();
		void RenderPitchControls();
		void RenderTimeControls();
		void RenderFilterControls();

		unsigned int SetColors();
	};


}