#pragma once

#include <GLFW/glfw3.h>
#include <lodepng.h>

#include "BackBeat/Core/Window.h"
namespace BackBeat {

#define NUM_LOGOS 4

	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();

		void OnUpdate() override;

		unsigned int GetWidth() const override { return m_Data.Width; }
		unsigned int GetHeight() const override { return m_Data.Height; }

		inline void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
		void SetVSync(bool enabled) override;
		bool IsVSync() const override;

		virtual void* GetNativeWindow() const { return m_Window; }

	private:
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();
		
		void LoadIcons();

	private:
		GLFWwindow* m_Window;

		struct WindowData
		{
			std::string Title;
			unsigned int Width, Height;
			bool VSync;

			EventCallbackFn EventCallback;
		};

		WindowData m_Data;
		unsigned char* m_Icons[NUM_LOGOS];
		GLFWimage m_Images[NUM_LOGOS];
	};
}