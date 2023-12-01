#pragma once

// TODO: 
//  Make PlayBack
//  Create way to save GUI settings after closing app
//  Create custom title bar
//	Create custom icons

#include <BackBeat.h>
#include <imgui.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>
#include <GLFW/glfw3.h>

#include "Synthesizer.h"
#include "Playback.h"
class MainLayer : public BackBeat::Layer
{
	public:
		MainLayer(BackBeat::Window* window);
		~MainLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate() override;
		virtual void OnEvent(BackBeat::Event& event) override;
		virtual void OnImGuiRender() override;

	private:
		BackBeat::Window* m_Window;
		Synthesizer m_Synth;
		// Playback m_Player;

		BackBeat::WindowsRenderer m_AudioRenderer;


		bool OnKeyEvent(BackBeat::KeyPressedEvent& event);
		bool OnMouseButtonEvent(BackBeat::MouseButtonPressedEvent& event);
};