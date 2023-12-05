#include "Playback.h"

	Playback::Playback()
		: m_Open(true)
	{
	
	}

	Playback::~Playback() 
	{
		m_Player.Stop();
	}

	void Playback::Update() 
	{

	}

	void Playback::OnEvent(BackBeat::Event& event) 
	{
		BackBeat::EventDispatcher dispatcher(event);
		dispatcher.Dispatch<BackBeat::KeyPressedEvent>(BIND_EVENT_FN(Playback::OnKeyEvent));
		dispatcher.Dispatch<BackBeat::MouseButtonPressedEvent>(BIND_EVENT_FN(Playback::OnMouseButtonEvent));
	}

	// TODO: Print out file/song name w/o directory
	//       Create a makeshift selectable progress bar
	//       Add a volume bar
	void Playback::ImGuiRender() 
	{
		if (!m_Open) {
			m_Player.Pause();
			return;
		}

		const ImGuiViewport* mainViewport = ImGui::GetMainViewport();
		float x = mainViewport->WorkPos.x;
		float y = mainViewport->WorkPos.y;
		ImGui::SetNextWindowPos(ImVec2(x, y), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize(ImVec2(600.0f, 150.0f), ImGuiCond_Once);
		// Playback flags
		ImGuiWindowFlags play_window_flags = 0;
		play_window_flags |= ImGuiWindowFlags_NoCollapse;
		play_window_flags |= ImGuiWindowFlags_MenuBar;
		play_window_flags |= ImGuiWindowFlags_NoResize;

		unsigned int count = SetPlaybackColors();

		ImGui::Begin("Player", &m_Open, play_window_flags);

		// Render MenuBar
		{
			if (ImGui::BeginMenuBar())
			{
				if (ImGui::BeginMenu("Menu"))
				{
					if (ImGui::MenuItem("Open"))
					{
						m_Player.Pause();
						m_Player.LoadTrack(BackBeat::FileDialog::OpenFile());
					}
					ImGui::EndMenu();
				}
				ImGui::EndMenuBar();
			}
		}

		// Render Playback controls
		{
			if (!m_Player.IsPlaying()) 
			{
				if (ImGui::Button("Play", ImVec2(50, 20)))
						m_Player.Start();
			}
			else
			{
				if (ImGui::Button("Pause", ImVec2(50, 20)))
					m_Player.Pause();
			} ImGui::SameLine();
		
			const int charLimit = 80;
			static char trackName[charLimit];
			strcpy_s(trackName, m_Player.GetTrackName().c_str());
			ImGui::Text(trackName);

			BackBeat::TimeMinSec trackTime = m_Player.GetTime();
			BackBeat::TimeMinSec trackLength = m_Player.GetLength();
			float progress = m_Player.GetProgress();

			ImGui::Text("%d:%02d", trackTime.minutes, trackTime.seconds); ImGui::SameLine();
			ImGui::ProgressBar(progress, ImVec2(0.0f, 0.0f), ""); ImGui::SameLine();
			ImGui::Text("%d:%02d", trackLength.minutes, trackLength.seconds);

			ImGui::Spacing();

			static float volume = 1.0f;
			ImGui::Text("    "); ImGui::SameLine(); ImGui::SliderFloat("Volume", &volume, 0.0f, 1.0f);
			m_Player.SetVolume(volume);
		}

		ImGui::End();

		ImGui::PopStyleColor(count);
	}

	void Playback::Open() 
	{
		m_Open = true;
	}

	void Playback::Close() 
	{
		m_Player.Pause();
		m_Open = false;
	}

	bool Playback::OnKeyEvent(BackBeat::KeyPressedEvent& event)
	{
		if (m_Player.IsLoaded() && event.GetKeyCode() == BackBeat::Key::Space)
		{
			if (!m_Player.IsPlaying()) {
				m_Player.Start();
			}
			else {
				m_Player.Pause();
			}
			event.Handled = true;
			return true;
		}
		return false;
	}

	bool Playback::OnMouseButtonEvent(BackBeat::MouseButtonPressedEvent& event)
	{
		return false;
	}

	// Playback color codes:
	// Table background:  #26181D
	// Window background: #382C31
	// Menu bar:          #735B63
	// Progress bar:      #e03d86
	unsigned int Playback::SetPlaybackColors()
	{
		unsigned int count = 0;

		// MenuBar colors
		ImGui::PushStyleColor(ImGuiCol_MenuBarBg, IM_COL32(115, 91, 99, 255)); count++;

		// Window colors
		ImGui::PushStyleColor(ImGuiCol_WindowBg, IM_COL32(56, 44, 49, 255)); count++;
		ImGui::PushStyleColor(ImGuiCol_Border, IM_COL32(115, 91, 99, 255)); count++;

		// Misc colors
		ImGui::PushStyleColor(ImGuiCol_PlotHistogram, IM_COL32(224, 61, 133, 255)); count++;
		ImGui::PushStyleColor(ImGuiCol_SliderGrab, IM_COL32(224, 61, 133, 255)); count++;
		ImGui::PushStyleColor(ImGuiCol_SliderGrabActive, IM_COL32(224, 61, 133, 255)); count++;

		return count;
	}