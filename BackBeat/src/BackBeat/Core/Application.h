#pragma once

//Application class for applications using the BackBeat backend as an application engine

#include "Core.h"
#include "LayerStack.h"
#include "Window.h"
#include "BackBeat/Events/Event.h"
#include "BackBeat/Events/ApplicationEvent.h"
#include "BackBeat/ImGui/ImGuiLayer.h"
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

		ImGuiLayer* GetImGuiLayer() { return m_ImGuiLayer; }

		Window& GetWindow() { return *m_Window; }

		static Application& Get() { return *s_Instance; }

	private:
		bool OnWindowClose(WindowCloseEvent& e);

	private:
		static Application* s_Instance;
		ImGuiLayer* m_ImGuiLayer;
		Window* m_Window;
		bool m_Running = true;
		LayerStack m_LayerStack;
	};

	// To be defined in CLIENT
	Application* CreateApplication();

}