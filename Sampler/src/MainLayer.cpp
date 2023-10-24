 #include "MainLayer.h"

	MainLayer::MainLayer(BackBeat::Window* window)
		: Layer("MainLayer"),
		m_Window(window),
		m_Worker{}
	{
	}

	MainLayer::~MainLayer()
	{
		m_Synth->Stop();
	}

	void MainLayer::OnAttach()
	{
		m_Synth = std::make_unique<BackBeat::Synth>();
		m_SynthEventHandler = m_Synth->GetEventHandler();
	}

	void MainLayer::OnDetach()
	{
	}
	
	void MainLayer::OnUpdate(DWORD timeInterval)
	{
	}

	void MainLayer::OnEvent(BackBeat::Event& event) 
	{
		BackBeat::EventDispatcher dispatcher(event);

		dispatcher.Dispatch<BackBeat::KeyPressedEvent>(BIND_EVENT_FN(MainLayer::OnKeyEvent));
		dispatcher.Dispatch<BackBeat::MouseButtonPressedEvent>(BIND_EVENT_FN(MainLayer::OnMouseButtonEvent));

		if (m_Synth->IsRunning())
			m_SynthEventHandler->HandleEvent(event);

		event.Handled = true;
	}

	void MainLayer::OnImGuiRender() 
	{	
		unsigned int width = m_Window->GetWidth();
		unsigned int height = m_Window->GetHeight();
		ImGui::SetNextWindowSize(ImVec2((float)width, (float)height));
		ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f), (ImGuiCond)0);

		// Flags
		ImGuiWindowFlags window_flags = 0;
		window_flags |= ImGuiWindowFlags_NoTitleBar;
		window_flags |= ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoCollapse;
		window_flags |= ImGuiWindowFlags_NoResize;

		bool open;
		ImGui::Begin("Sampler", &open, window_flags);
		
		ImGui::Spacing();
		ImGui::Text("PRESS SPACE TO PLAY");
		ImGui::Separator();

		ImGui::SeparatorText("WHITE KEYS:");
		ImGui::Text(" NOTES:");   ImGui::SameLine(); ImGui::Text("KEY:");
		ImGui::BulletText("C  "); ImGui::SameLine(); ImGui::BulletText("A");
		ImGui::BulletText("D  "); ImGui::SameLine(); ImGui::BulletText("S");
		ImGui::BulletText("E  "); ImGui::SameLine(); ImGui::BulletText("D");
		ImGui::BulletText("F  "); ImGui::SameLine(); ImGui::BulletText("F");
		ImGui::BulletText("G  "); ImGui::SameLine(); ImGui::BulletText("G");
		ImGui::BulletText("A  "); ImGui::SameLine(); ImGui::BulletText("H");
		ImGui::BulletText("B  "); ImGui::SameLine(); ImGui::BulletText("J");
		ImGui::BulletText("C  "); ImGui::SameLine(); ImGui::BulletText("K");
		ImGui::Spacing();

		ImGui::SeparatorText("BLACK KEYS:");
		ImGui::Text(" NOTES:    ");   ImGui::SameLine(); ImGui::Text("KEY:");
		ImGui::BulletText("C Sharp"); ImGui::SameLine(); ImGui::BulletText("W");
		ImGui::BulletText("D Sharp"); ImGui::SameLine(); ImGui::BulletText("E");
		ImGui::BulletText("F Sharp"); ImGui::SameLine(); ImGui::BulletText("T");
		ImGui::BulletText("G Sharp"); ImGui::SameLine(); ImGui::BulletText("Y");
		ImGui::BulletText("A Sharp"); ImGui::SameLine(); ImGui::BulletText("U");
		ImGui::Spacing();

		ImGui::SeparatorText("MISC. CONTROLS");
		ImGui::Text(" Octaves:       ");   ImGui::SameLine(); ImGui::Text("KEY:");
		ImGui::BulletText("Octave up   "); ImGui::SameLine(); ImGui::BulletText("Up");
		ImGui::BulletText("Octave down "); ImGui::SameLine(); ImGui::BulletText("Down");
		ImGui::Spacing();

		ImGui::Text(" Note Velocity:    ");   ImGui::SameLine(); ImGui::Text("KEY:");
		ImGui::BulletText("Velocity up    "); ImGui::SameLine(); ImGui::BulletText("Right");
		ImGui::BulletText("Velocity down  "); ImGui::SameLine(); ImGui::BulletText("Left");

		ImGui::End();
	}

	bool MainLayer::OnKeyEvent(BackBeat::KeyPressedEvent& event)
	{

		if (event.isRepeat()) {
			event.Handled = true;
			return true;
		}
		
		switch (event.GetKeyCode())
		{

		case BackBeat::Key::Space:
		{
			if (!m_Synth->IsRunning()) {
				BB_CLIENT_TRACE("Synth started");
				m_Synth->Start();
			}
			else {
				BB_CLIENT_TRACE("Synth stopped");
				m_Synth->Stop();
			}
			break;
		}

		}

		event.Handled = true;
		return true;
	}

	bool MainLayer::OnMouseButtonEvent(BackBeat::MouseButtonPressedEvent& event)
	{
		event.Handled = true;
		return true;
	}