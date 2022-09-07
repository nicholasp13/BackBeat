#pragma once

#include "Core.h"
#include "LayerStack.h""
#include "Events/Event.h"
#include "Events/ApplicationEvent.h"
#include "Window.h"

//Application class for applications using the BackBeat engine

namespace BackBeat {

	class BACKBEAT_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

	private:
		bool OnWindowClose(WindowCloseEvent& e);

	private:
		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
		LayerStack m_LayerStack;
	};

	// TO be defined in CLIENT
	Application* CreateApplication();

}