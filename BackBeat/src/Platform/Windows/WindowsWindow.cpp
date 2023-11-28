#include "bbpch.h"

#include <glad/glad.h>

#include "BackBeat/Events/ApplicationEvent.h"
#include "BackBeat/Events/KeyEvent.h"
#include "BackBeat/Events/MouseEvent.h"
#include "WindowsWindow.h"

namespace BackBeat {

	static bool s_GLFWInitialized = false;
	static void GLFWErrorCallback(int error, const char* description)
	{
		BB_CORE_ERROR("GLFW Error ({0})", error, description);
	}

	Window* Window::Create(const WindowProps& props)
	{
		return new WindowsWindow(props);
	}

	WindowsWindow::WindowsWindow(const WindowProps& props)
	{
		Init(props);
	}

	WindowsWindow::~WindowsWindow()
	{
		Shutdown();
	}

	void WindowsWindow::Init(const WindowProps& props)
	{
		if (!s_GLFWInitialized)
		{
			int success = glfwInit();
			BB_CORE_ASSERTS(success, "Could not initialize GLFW!");
			glfwSetErrorCallback(GLFWErrorCallback);
			s_GLFWInitialized = true;
		}

		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;
		BB_CORE_INFO("Creating window {0} ({1}, {2})", m_Data.Title, m_Data.Width, m_Data.Height);
		m_Window = glfwCreateWindow((int)m_Data.Width, (int)m_Data.Height, m_Data.Title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(m_Window);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		BB_CORE_ASSERTS(status, "Failed to initialize Glad!")
		glfwSetWindowUserPointer(m_Window, &m_Data);
		SetVSync(true);

		// Set Windows icons for BackBeat app
		LoadIcons();
		glfwSetWindowIcon(m_Window, NUM_LOGOS, m_Images);

		// Set GLFW callbacks
		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				data.Width = width;
				data.Height = height;

				WindowResizeEvent event(width, height);
				data.EventCallback(event);
			});

		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				WindowCloseEvent event;
				data.EventCallback(event);
			});

		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				switch (action)
				{
					case GLFW_PRESS:
					{
						KeyPressedEvent event(key, 0);
						data.EventCallback(event);
						break;
					}
					case GLFW_RELEASE:
					{
						KeyReleasedEvent event(key);
						data.EventCallback(event);
						break;
					}
					case GLFW_REPEAT:
					{
						KeyPressedEvent event(key, 1);
						data.EventCallback(event);
						break;
					}
				}
			});

		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
			{
			WindowData & data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action)
			{
				case GLFW_PRESS:
				{
					MouseButtonPressedEvent event(button);
					data.EventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					MouseButtonReleasedEvent event(button);
					data.EventCallback(event);
					break;
				}
			}
		});

		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				MouseScrolledEvent event((float)xOffset, (float)yOffset);
				data.EventCallback(event);
			});


		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				MouseMovedEvent	event((float)xOffset, (float)yOffset);
				data.EventCallback(event);
			});
	}

	void WindowsWindow::Shutdown()
	{
		glfwDestroyWindow(m_Window);
		glfwTerminate();
	}

	void WindowsWindow::OnUpdate()
	{
		glfwPollEvents();
		glfwSwapBuffers(m_Window);
	}

	void WindowsWindow::SetVSync(bool enabled)
	{
		if (enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);
	}

	bool WindowsWindow::IsVSync() const
	{
		return m_Data.VSync;
	}



	void WindowsWindow::LoadIcons()
	{

		int index = 0;
		unsigned width = 0;
		unsigned height = 0;

		lodepng_decode32_file(m_Icons + index, &width, &height, "../BackBeat/assets/logos/BackbeatLogo_NoFilter_16x16.png");
		m_Images[index].width = width;
		m_Images[index].height = height;
		m_Images[index].pixels = m_Icons[index];
		index++;

		lodepng_decode32_file(m_Icons + index, &width, &height, "../BackBeat/assets/logos/BackbeatLogo_NoFilter_32x32.png");
		m_Images[index].width = width;
		m_Images[index].height = height;
		m_Images[index].pixels = m_Icons[index];
		index++;

		lodepng_decode32_file(m_Icons + index, &width, &height, "../BackBeat/assets/logos/BackbeatLogo_NoFilter_48x48.png");
		m_Images[index].width = width;
		m_Images[index].height = height;
		m_Images[index].pixels = m_Icons[index];
		index++;

		lodepng_decode32_file(m_Icons + index, &width, &height, "../BackBeat/assets/logos/BackbeatLogo_NoFilter_256x256.png");
		m_Images[index].width = width;
		m_Images[index].height = height;
		m_Images[index].pixels = m_Icons[index];
	}
}