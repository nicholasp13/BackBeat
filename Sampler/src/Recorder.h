#pragma once

// NOTE: This is for Windows API recording input devices. Future implementation will requrire recorders for all
//       BackBeat audio objects so this file maybe renamed/refactored to accomandate such implementations

#include <BackBeat.h>
#include <imgui.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>
#include <GLFW/glfw3.h>
namespace BBSampler {
	
	constexpr const char* WindowsTempFileLocaation = "temp.txt"; // NOTE: Placeholder temp location. Will use appdata, roaming, or cache folder in the future.

	class Recorder
	{
	public:
		Recorder();
		~Recorder();

		void Update();
		void OnEvent(BackBeat::Event& event);
		void ImGuiRender();
		void Open();
		void Close();

		inline bool IsOpen() { return m_Open; }

	private:
		bool m_Open;
		BackBeat::WindowsRecorder m_WindowsRecorder;

		bool OnKeyEvent(BackBeat::KeyPressedEvent& event);
		bool OnMouseButtonEvent(BackBeat::MouseButtonPressedEvent& event);
		unsigned int SetRecorderColors();
	};
}