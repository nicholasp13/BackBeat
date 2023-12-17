#include "MainLayer.h"

	MainLayer::MainLayer(BackBeat::Window* window)
		: Layer("MainLayer"), m_Window(window)
	{
	}

	MainLayer::~MainLayer()
	{

	}

	void MainLayer::OnAttach()
	{
		m_Synth.Init();
		m_AudioRenderer.GetMixer()->SetProc(m_Synth.GetSynthProc());
		m_AudioRenderer.GetMixer()->SetProc(m_Player.GetProc());
		m_AudioRenderer.Start();
	}

	void MainLayer::OnDetach()
	{
		m_Synth.Close();
		m_Player.Close();
		m_AudioRenderer.Stop();
	}
	
	void MainLayer::OnUpdate()
	{
		m_Synth.Update();
		m_Player.Update();
	}

	void MainLayer::OnEvent(BackBeat::Event& event) 
	{
		m_Synth.OnEvent(event);
		m_Player.OnEvent(event);

		BackBeat::EventDispatcher dispatcher(event);
		dispatcher.Dispatch<BackBeat::KeyPressedEvent>(BIND_EVENT_FN(MainLayer::OnKeyEvent));
		dispatcher.Dispatch<BackBeat::MouseButtonPressedEvent>(BIND_EVENT_FN(MainLayer::OnMouseButtonEvent));
		event.Handled = true;
	}

	void MainLayer::OnImGuiRender() 
	{	
		unsigned int width = m_Window->GetWidth();
		unsigned int height = m_Window->GetHeight(); 

		// Render background
		{
			ImGui::SetNextWindowSize(ImVec2((float)width, (float)height));
			ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f), (ImGuiCond)0);
			// Background flags
			ImGuiWindowFlags window_flags = 0;
			window_flags |= ImGuiWindowFlags_NoTitleBar;
			window_flags |= ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoCollapse;
			window_flags |= ImGuiWindowFlags_NoResize;
			window_flags |= ImGuiWindowFlags_MenuBar;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
			bool open;

			ImGui::Begin("Background", &open, window_flags);

			// Render MenuBar
			if (ImGui::BeginMenuBar())
			{
				if (ImGui::BeginMenu("Player"))
				{
					if (ImGui::MenuItem("Open"))
					{
						m_Player.Open();
					}
					ImGui::EndMenu();
				}
				if (ImGui::BeginMenu("Synth"))
				{
					if (ImGui::MenuItem("Open"))
					{
						m_Synth.Open();
					}
					ImGui::EndMenu();
				}
				ImGui::EndMenuBar();
			}

			ImGui::End();
		}

		m_Synth.ImGuiRender();
		m_Player.ImGuiRender();

		// ImGui::ShowDemoWindow();
	}

	bool MainLayer::OnKeyEvent(BackBeat::KeyPressedEvent& event)
	{
		if (event.isRepeat()) {
			event.Handled = true;
			return true;
		}
		event.Handled = true;
		return true;
	}

	bool MainLayer::OnMouseButtonEvent(BackBeat::MouseButtonPressedEvent& event)
	{
		event.Handled = true;
		return true;
	}