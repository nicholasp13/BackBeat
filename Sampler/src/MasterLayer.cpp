 #include "MasterLayer.h"

	MasterLayer::MasterLayer(BackBeat::Window* window)
		: Layer("MasterLayer"),
		m_Player(BackBeat::Player("assets/audio/LearnToFly.wav")),
		m_Window(window)
	{
	}

	MasterLayer::~MasterLayer()
	{
	}

	void MasterLayer::OnAttach()
	{
	}

	void MasterLayer::OnDetach()
	{
	}
	
	void MasterLayer::OnUpdate(DWORD timeInterval)
	{
	}

	// TODO: Key Codes and Mouse Codes
	void MasterLayer::OnEvent(BackBeat::Event& event) 
	{
		BackBeat::EventDispatcher dispatcher(event);

		dispatcher.Dispatch<BackBeat::KeyPressedEvent>(BIND_EVENT_FN(MasterLayer::OnKeyEvent));

		if (event.IsInCategory(BackBeat::EventCategoryMouse)) {
			BB_CLIENT_TRACE("MouseEvent: {0}", event);
		}
		/**/

		event.Handled = true;
	}

	void MasterLayer::OnImGuiRender() 
	{	
		unsigned int width = m_Window->GetWidth();
		unsigned int height = m_Window->GetHeight();
		ImGui::SetNextWindowSize(ImVec2(width, height));
		ImGui::SetNextWindowPos(ImVec2(0, 0), 0);

		// Flags
		ImGuiWindowFlags window_flags = 0;
		window_flags |= ImGuiWindowFlags_NoTitleBar;
		window_flags |= ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoCollapse;
		window_flags |= ImGuiWindowFlags_NoResize;

		bool open;
		ImGui::Begin("Sampler", &open, window_flags);
		ImGui::Text("PRESS ANY KEY TO PLAY");

		ImGui::End();
	}

	bool MasterLayer::OnKeyEvent(BackBeat::KeyPressedEvent& event)
	{
		if (event.isRepeat()) {
			return false;
			event.Handled = true;
		}

		BB_CLIENT_TRACE("KeyEvent: {0}", event);

		if (!m_Player.Playing) {
			BB_CLIENT_TRACE("Playing started");
			std::thread worker(&BackBeat::Player::Play, &m_Player);
			worker.detach();
		}
		else if (m_Player.Playing) {
			BB_CLIENT_TRACE("Playing paused");
			m_Player.Pause();
		}
		event.Handled = true;

		return true;
	}

	bool MasterLayer::OnMouseButtonEvent(BackBeat::MouseButtonPressedEvent& event)
	{
		BB_CLIENT_TRACE("MouseEvent: {0}", event);
		event.Handled = true;

		return true;
	}