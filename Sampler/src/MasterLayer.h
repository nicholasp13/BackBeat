#pragma once

#include <BackBeat.h>
#include <imgui.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>
#include <GLFW/glfw3.h>
class MasterLayer : public BackBeat::Layer
{
	public:
		
		MasterLayer(BackBeat::Window* window);
		~MasterLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate(DWORD timeInterval) override;
		virtual void OnEvent(BackBeat::Event& event) override;
		virtual void OnImGuiRender() override;

	private:
		bool OnKeyEvent(BackBeat::KeyPressedEvent& event);
		bool OnMouseButtonEvent(BackBeat::MouseButtonPressedEvent& event);
		
		float m_Octave;
		float m_Volume;

		std::thread m_Worker;

		BackBeat::Window* m_Window;
		BackBeat::Player* m_Player;
		BackBeat::Oscillator* m_Oscillator;
		BackBeat::Loader* m_Loader;
		BackBeat::Renderer* m_Renderer;
		BackBeat::AudioData* m_AudioData;
};