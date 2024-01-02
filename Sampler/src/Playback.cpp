#include "Playback.h"

	Playback::Playback()
		: m_Open(false)
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
						m_Player.LoadTrack(BackBeat::FileDialog::OpenFile("WAV Files (*.wav)\0*.wav\0"));
					}
					if (m_Player.IsLoaded())
					{
						if (ImGui::MenuItem("Save"))
						{
							m_Player.Pause();
							unsigned int position = m_Player.GetPosition();
							unsigned int startPosition = m_Player.GetStartPosition();
							unsigned int endPosition = m_Player.GetEndPosition();
							BackBeat::AudioFileBuilder::BuildWAVFile(m_Player.GetTrack(), startPosition, endPosition);
							m_Player.SetPosition(position);
						}
					}
					else
					{
						ImGui::MenuItem("Save", NULL, false, false);
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
			
			int pos1 = m_Player.GetPosition();
			int size = m_Player.GetSize();
			static bool wasPlaying = false;
			ImGui::Text("%d:%02d", trackTime.minutes, trackTime.seconds); ImGui::SameLine();
			
			// NOTE: - Future implementation might want the track to keep playing while visually the seek bar changes
			//           and only when the bar is released then the position is changed. (This does not work as either
			//           the bar does not update while the track plays but does change only when released OR the
			//           bar does not move or change with the track position)
			//       - Removing the Pause() creates an annoying effect where the sound at the position is repeatedly playing
			if (m_Player.IsLoaded()) 
			{
				ImGui::PushID("Seekbar");
				if (BackBeat::ImGuiWidgets::ImGuiSeekBarInt("##", &pos1, m_Player.GetSize(), "", ImGuiSliderFlags(0))) 
				{
					if (m_Player.IsPlaying()) 
					{
						m_Player.Pause();
						wasPlaying = true;
					}
					m_Player.SetPosition(pos1);
				}
				if (ImGui::IsItemDeactivated() && wasPlaying) 
				{
					m_Player.Play();
					wasPlaying = false;
				}
				ImGui::SameLine(); ImGui::Text("%d:%02d", trackLength.minutes, trackLength.seconds);
				ImGui::PopID();

				ImGui::PushID("TrackEditor");
				const int zero = 0;
				float byteRate = (float)m_Player.GetByteRate();
				static int start = 0;
				static int end = size;
				BackBeat::TimeMinSec startTime = BackBeat::Audio::GetTime((float)start / byteRate);
				BackBeat::TimeMinSec endTime = BackBeat::Audio::GetTime((float)end / byteRate);
				ImGui::Text("%d:%02d", startTime.minutes, startTime.seconds); ImGui::SameLine();
				if (BackBeat::ImGuiWidgets::ImGuiTrackEditor("##", &start, &end, &zero, &size, "", ImGuiSliderFlags(0))) 
				{
					m_Player.SetStart(start);
					m_Player.SetEnd(end);
				}
				ImGui::SameLine(); ImGui::Text("%d:%02d", endTime.minutes, endTime.seconds);

				ImGui::PopID();
			}
			else 
			{
				// Renders an empty, uninteractable seek bar if no track is loaded
				ImGui::PushID("EmptySeekbar");
				int temp = 0;
				BackBeat::ImGuiWidgets::ImGuiSeekBarInt("##", &temp, 10000, "", ImGuiSliderFlags(0));
				ImGui::SameLine(); ImGui::Text("%d:%02d", trackLength.minutes, trackLength.seconds);
				ImGui::PopID();

				// Renders an empty, uninteractable track editor
				ImGui::PushID("EmptyTrackEditor");
				ImGui::Text("%d:%02d", 0, 0); ImGui::SameLine();
				BackBeat::ImGuiWidgets::ImGuiTrackEditor("##", &temp, &temp, &temp, &temp, "", ImGuiSliderFlags(0));
				ImGui::SameLine(); ImGui::Text("%d:%02d", 0, 0);
				ImGui::PopID();
			}

			ImGui::Spacing();

			static float volume = 1.0f;
			ImGui::Text("    "); ImGui::SameLine(); 
			BackBeat::ImGuiWidgets::ImGuiSeekBarFloat ("Volume", &volume, 1.0f, "", ImGuiSliderFlags(0));
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
			if (!m_Player.IsPlaying()) 
			{
				m_Player.Start();
			}
			else 
			{
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