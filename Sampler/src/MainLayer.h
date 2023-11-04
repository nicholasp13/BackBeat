#pragma once

// TODO: 
//	Create ImGui knobs for more classic synth GUI
//  Implement ModMatrix GUI after creating ModMatrix

#include <BackBeat.h>
#include <imgui.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>
#include <GLFW/glfw3.h>
class MainLayer : public BackBeat::Layer
{
	public:
		
		MainLayer(BackBeat::Window* window);
		~MainLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate(DWORD timeInterval) override;
		virtual void OnEvent(BackBeat::Event& event) override;
		virtual void OnImGuiRender() override;

	private:
		std::thread m_Worker;
		// Playback
		std::shared_ptr<BackBeat::Player> m_Player; // Not used
		std::shared_ptr<BackBeat::AudioData> m_AudioData; // Not used
		std::shared_ptr<BackBeat::Loader> m_Loader; // Not used
		// Synth
		std::unique_ptr<BackBeat::Synth> m_Synth;
		std::shared_ptr<BackBeat::SynthEventHandler> m_SynthEventHandler;
		std::shared_ptr<BackBeat::SynthParameters> m_SynthParams;

		BackBeat::Window* m_Window;

		bool OnKeyEvent(BackBeat::KeyPressedEvent& event);
		bool OnMouseButtonEvent(BackBeat::MouseButtonPressedEvent& event);

		void RenderInfo();
		void RenderControls(); // TODO: Change some sliders to knobs for more classic synth look
		void RenderModMatrix();// TODO: Implement after creating ModMatrix
};