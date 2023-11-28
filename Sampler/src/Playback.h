#pragma once

#include <BackBeat.h>
#include <imgui.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>
#include <GLFW/glfw3.h>
class PlaybackLayer : public BackBeat::Layer
{
	public:
		PlaybackLayer(BackBeat::Window* window);
		~PlaybackLayer();

		void Init();
		void Update();
		void OnEvent(BackBeat::Event& event);
		void ImGuiRender();
		void Open();
		void Close();

	private:
		
};