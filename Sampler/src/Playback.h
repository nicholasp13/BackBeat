#pragma once

// TODO: Create editable timeline for track objects

#include <BackBeat.h>
#include <imgui.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>
#include <GLFW/glfw3.h>
class Playback
{
	public:
		Playback();
		~Playback();

		void Update();
		void OnEvent(BackBeat::Event& event);
		void ImGuiRender();
		void Open();
		void Close();

		std::shared_ptr<BackBeat::PlayerProcessor> GetProc() { return m_Player.GetProc(); }

	private:
		bool m_Open;
		BackBeat::Player m_Player;

		bool OnKeyEvent(BackBeat::KeyPressedEvent& event);
		bool OnMouseButtonEvent(BackBeat::MouseButtonPressedEvent& event);
		unsigned int SetPlaybackColors();
};