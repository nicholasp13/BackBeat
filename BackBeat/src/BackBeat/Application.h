#pragma once

#include "Core.h"
#include "Events/ApplicationEvent.h"
#include "Log.h"
#include "Window.h"

//Application class for applications using the BackBeat engine

namespace BackBeat {

	class BACKBEAT_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
	private:
		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
	};

	// TO be defined in CLIENT
	Application* CreateApplication();

}