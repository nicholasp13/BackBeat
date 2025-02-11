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
		const float m_Width = 900.0f;
		const float m_Height = 200.0f;

		bool m_Open;
		bool m_Looping;

		int m_Zero;
		int m_Start;
		int m_End;
		int m_Size;
		int m_StartMs;
		int m_EndMs;
		float m_Volume;

		BackBeat::Player m_TrackPlayer;

	};


}