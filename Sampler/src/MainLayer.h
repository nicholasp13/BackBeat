#pragma once

// TODO: 
//  Create way to save GUI settings after closing app
//  Create custom title bar
//	Create custom icons

#include <BackBeat.h>
#include <imgui.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>
#include <GLFW/glfw3.h>

#include "Playback.h"
#include "SamplerController.h"
#include "Synthesizer.h"
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
		unsigned int m_NumMIDIDevices;
		std::vector< std::string > m_DeviceNames;

		BackBeat::Window* m_Window;
		SamplerController m_SamplerController;
		Synthesizer m_Synth;
		Playback m_Player;
		BackBeat::WindowsRenderer m_AudioRenderer;
		BackBeat::WindowsMIDIDeviceManager m_MIDIDeviceManager;

		bool OnKeyEvent(BackBeat::KeyPressedEvent& event);
		bool OnMouseButtonEvent(BackBeat::MouseButtonPressedEvent& event);
};