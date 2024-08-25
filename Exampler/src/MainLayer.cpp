// TODO: Create editable timeline for entity/track objects

#include "MainLayer.h"
namespace Exampler {

	MainLayer::MainLayer(BackBeat::Window* window, BackBeat::AudioSystem* audio)
		: 
		Layer("MainLayer"), 
		m_NumMIDIDevices(0),
		m_NumSynths(0),
		m_NumSamplers(0),
		m_NumPlayback(0),
		m_NumRecorders(0),
		m_EtyToRename(nullptr),
		m_EtyToDelete(nullptr),
		m_Window(window),
		m_Audio(audio),
		m_PlayerMgr(audio->GetPlayerManager()),
		m_RecorderMgr(audio->GetRecorderManager()),
		m_AudioRenderer(audio->GetRenderer()),
		m_MIDIDeviceManager(audio->GetMIDIDeviceManager()),
		m_Visualizer(audio->GetVisualizer())
	{

	}

	MainLayer::~MainLayer()
	{

	}

	void MainLayer::OnAttach()
	{
		m_NumMIDIDevices = m_MIDIDeviceManager->GetNumDevices();
		for (unsigned int i = 0; i < m_NumMIDIDevices; i++) {
			m_DeviceNames.push_back(m_MIDIDeviceManager->GetDeviceName(i));
		}

		// FOR TESTING
#ifdef TRUE
		m_Entities.push_back(std::make_shared<Dummy>());
#endif
	}

	void MainLayer::OnDetach()
	{
		for (auto itr = m_Entities.begin(); itr != m_Entities.end(); itr++)
		{
			(*itr)->Close();
			(*itr)->Off();
			(*itr)->Delete(m_PlayerMgr, m_RecorderMgr, m_AudioRenderer->GetMixer(), m_MIDIDeviceManager);
		}

	}

	void MainLayer::OnUpdate()
	{
		for (auto itr = m_Entities.begin(); itr != m_Entities.end(); itr++)
		{
			(*itr)->Update();
		}
		m_Visualizer->Update();
	}

	void MainLayer::OnEvent(BackBeat::Event& event)
	{
		for (auto itr = m_Entities.begin(); itr != m_Entities.end(); itr++)
		{
			(*itr)->OnEvent(event);
		}

		BackBeat::EventDispatcher dispatcher(event);
		dispatcher.Dispatch<BackBeat::KeyPressedEvent>(BIND_EVENT_FN(MainLayer::OnKeyEvent));
		dispatcher.Dispatch<BackBeat::MouseButtonPressedEvent>(BIND_EVENT_FN(MainLayer::OnMouseButtonEvent));
		event.Handled = true;
	}

	void MainLayer::OnImGuiRender()
	{
		unsigned int width = m_Window->GetWidth();
		unsigned int height = m_Window->GetHeight();
		if (width == 0 || height == 0)
			return;

		unsigned int count = SetMainColors();

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

			RenderMenubar();
			RenderCanvas();
			RenderAudioVisualizer();
			RenderMgrs();
			RenderPopups();

			ImGui::End();
		}

		ImGui::PopStyleColor(count);
	}

	void MainLayer::RenderMenubar()
	{
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Menu"))
			{
				// Entities menu
				if (ImGui::BeginMenu("Tracks"))
				{
					if (m_Entities.size() == 0)
					{
						ImGui::BeginDisabled();
						if (ImGui::MenuItem("No tracks")) 
						{ }
						ImGui::EndDisabled();
					}
					
					for (unsigned int i = 0; i < m_Entities.size(); i++)
						RenderEntityMenubar(i);

					ImGui::EndMenu();
				}

				// MIDI Devices menu
				if (ImGui::BeginMenu("MIDI Devices"))
				{
					if (m_NumMIDIDevices == 0)
					{
						ImGui::BeginDisabled();
						if (ImGui::MenuItem("No devices detected"))
						{ }
						ImGui::EndDisabled();
					}

					for (unsigned int i = 0; i < m_NumMIDIDevices; i++) {

						// TODO: Test multiple MIDI devices and if MIDI devices change number
						bool s_DeviceOpen = m_MIDIDeviceManager->IsOpen(i);

						if (ImGui::BeginMenu(m_DeviceNames[i].c_str()))
						{
							if (ImGui::MenuItem("Set Active", "", &s_DeviceOpen))
							{
								if (s_DeviceOpen)
								{
									if (m_MIDIDeviceManager->OpenDevice(i))
										m_MIDIDeviceManager->RunDevice(i);
								}
								else
								{
									m_MIDIDeviceManager->StopDevice();
								}
							}

							ImGui::EndMenu();
						}
					}

					ImGui::EndMenu();
				}

				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}
	}

	void MainLayer::RenderCanvas()
	{
		unsigned int width = m_Window->GetWidth();
		unsigned int height = m_Window->GetHeight();
		const int wBorder = 20;
		const int hBorder = 250;
		ImGuiWindowFlags childFlags = 0;
		childFlags |= ImGuiWindowFlags_NoTitleBar;
		childFlags |= ImGuiWindowFlags_NoMove;
		childFlags |= ImGuiWindowFlags_NoCollapse;
		childFlags |= ImGuiWindowFlags_NoResize;
		childFlags |= ImGuiWindowFlags_AlwaysVerticalScrollbar;
		unsigned int cCount = SetCanvasColors();

		ImGui::BeginChild("Canvas", ImVec2((float)(width - wBorder), (float)(height - hBorder)), false, childFlags);

		// Render Entities
		if (m_Entities.size() == 0)
			ImGui::TextDisabled("Right click to add tracks");
		else
		{
			const float tWidth = (float)width;
			const float tHeight = 1200.0f;
			static ImGuiTableFlags tableFlags = 0;
			tableFlags |= ImGuiTableFlags_RowBg;
			tableFlags |= ImGuiTableFlags_BordersInner;
			tableFlags |= ImGuiTableFlags_NoPadOuterX;
			tableFlags |= ImGuiTableFlags_NoPadInnerX;
			ImGui::BeginTable("Entities", 1, tableFlags, ImVec2(tWidth, tHeight), 0.0f);

			RenderEntities();

			ImGui::EndTable();
		}
		ImGui::EndChild();

		// Render right click menu popup
		if (ImGui::BeginPopupContextItem("PopUpCreator", ImGuiPopupFlags_MouseButtonRight))
		{
			bool limitSynths = (m_NumSynths < MaxSynths);
			if (ImGui::MenuItem("Add Synth", "", false, limitSynths))
				AddSynth();

			bool limitSamplers = (m_NumSamplers < MaxSamplers);
			if (ImGui::MenuItem("Add Sampler", "", false, limitSamplers))
				AddSampler();

			bool limitPlayback = (m_NumPlayback < MaxPlayback);
			if (ImGui::MenuItem("Add Playback Track", "", false, limitPlayback))
				AddPlaybackTrack();

			bool limitRecorders = (m_NumRecorders < MaxRecordingDevices);
			if (ImGui::MenuItem("Add Recording Track", "", false, limitRecorders))
				AddRecordingTrack();
			
			ImGui::EndPopup();
		}

		ImGui::PopStyleColor(cCount);
	}

	// BUG: When spamming the on and off button. It may cause the visuals to glitch (mostly show the data when it should have been flushed)
	void MainLayer::RenderAudioVisualizer()
	{
		ImGui::Spacing();

		if (!m_Visualizer->IsOn())
		{
			if (ImGui::Button("On", ImVec2(100, 20)))
				m_Visualizer->On();
		}
		else
		{
			if (ImGui::Button("Off", ImVec2(100, 20)))
				m_Visualizer->Off();
		}

		// NOTE: While inuitive to think that the min/max shouldd be near 1.0f, the sound is multiplied by multiple different gain levels
		//       through the BackBeat workflow making the average peak of a sound wave much lower than you would think.
		const float visualMax = .15f;
		auto bufferSize = m_Visualizer->GetChannelBufferSize();
		for (unsigned int i = 0; i < m_Visualizer->GetNumChannels(); i++)
		{
			ImGui::Spacing();
			std::string name = "Channel " + std::to_string(i);
			ImGui::PlotLines(name.c_str(), m_Visualizer->GetChannelBuffer(i), bufferSize, 1, "", visualMax * -1, visualMax, ImVec2(m_Window->GetWidth() - 200.0f, 60.0f));
		}


		ImGui::Spacing();
	}

	void MainLayer::RenderMgrs()
	{
		if (!m_PlayerMgr->IsPlaying())
		{
			if (ImGui::Button("Play", ImVec2(100, 20)))
				if (!m_RecorderMgr->IsRecording())
					m_PlayerMgr->PlayAll();
		}
		else
		{
			if (ImGui::Button("Pause", ImVec2(100, 20)))
				m_PlayerMgr->PauseAll();
		} ImGui::SameLine();

		if (!m_RecorderMgr->IsRecording())
		{
			if (ImGui::Button("Record", ImVec2(100, 20)))
				if (!m_PlayerMgr->IsPlaying())
					m_RecorderMgr->Start();
		}
		else
		{
			if (ImGui::Button("Pause Recording", ImVec2(100, 20)))
				m_RecorderMgr->Stop();
		}

	}

	void MainLayer::RenderEntities()
	{
		for (auto itr = m_Entities.begin(); itr != m_Entities.end(); itr++)
		{
			ImGui::TableNextColumn();
			std::shared_ptr<Entity> entity = *itr;
			entity->ImGuiRender();
		}
	}

	void MainLayer::RenderEntityMenubar(unsigned int index)
	{
		std::shared_ptr<Entity> entity = m_Entities[index];
		auto type = entity->GetType();
		auto name = entity->GetName();
		auto id = entity->GetID();

		const std::string hashDivider = "###";
		std::string labelID = name.c_str() + hashDivider + id.ToString();
		if (ImGui::BeginMenu(labelID.c_str()))
		{
			if (type == EntityType::synth || type == EntityType::sampler)
				if (ImGui::MenuItem("Open"))
					entity->Open();

			if (ImGui::MenuItem("Rename"))
				m_EtyToRename = entity;

			if (ImGui::MenuItem("Delete"))
				m_EtyToDelete = entity;

			ImGui::EndMenu();
		}
	}

	void MainLayer::RenderPopups()
	{
		if (m_EtyToRename)
			ImGui::OpenPopup("RenamePopupID");
		if (ImGui::BeginPopup("RenamePopupID"))
		{
			char changeName[128] = {};
			bool opened = false;
			ImGui::InputTextWithHint("Input New Name", m_EtyToRename->GetName().c_str(), changeName, IM_ARRAYSIZE(changeName));
			if (ImGui::IsItemDeactivated())
			{
				auto newName = std::string(changeName);
				if (newName.size() > 0)
					m_EtyToRename->SetName(newName);
				m_EtyToRename = nullptr;
				ImGui::CloseCurrentPopup();
			}
			
			ImGui::EndPopup();
		}

		if (m_EtyToDelete)
			ImGui::OpenPopup("DeletePopupID");
		if (ImGui::BeginPopup("DeletePopupID"))
		{
			ImGui::Text("Are you sure you want to delete"); ImGui::SameLine();
			ImGui::Text(m_EtyToDelete->GetName().c_str()); ImGui::SameLine();
			ImGui::Text("?");

			if(ImGui::Button("Yes"))
			{
				DeleteEntity();
				ImGui::CloseCurrentPopup();
			} ImGui::SameLine();

			if (ImGui::Button("No"))
			{
				m_EtyToDelete = nullptr;
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}

	}

	void MainLayer::AddSynth()
	{
		if (m_NumSynths >= MaxSynths)
			return;
		m_Audio->Stop();

		auto synth = std::make_shared<Synthesizer>();
		synth->Add(m_PlayerMgr, m_RecorderMgr, m_AudioRenderer->GetMixer(), m_MIDIDeviceManager);
		std::string synthName = "Synth " + std::to_string(++m_NumSynths);
		synth->SetName(synthName);
		m_Entities.push_back(synth);
		
		m_Audio->Start();
	}

	void MainLayer::AddSampler()
	{
		if (m_NumSamplers >= MaxSamplers)
			return;
		m_Audio->Stop();

		auto sampler = std::make_shared<Sampler>();
		sampler->Add(m_PlayerMgr, m_RecorderMgr, m_AudioRenderer->GetMixer(), m_MIDIDeviceManager);
		std::string samplerName = "Sampler " + std::to_string(++m_NumSamplers);
		sampler->SetName(samplerName);
		m_Entities.push_back(sampler);

		m_Audio->Start();
	}

	void MainLayer::AddPlaybackTrack()
	{
		if (m_NumPlayback >= MaxPlayback)
			return;
		m_Audio->Stop();

		auto playback = std::make_shared<PlaybackTrack>();
		playback->Add(m_PlayerMgr, m_RecorderMgr, m_AudioRenderer->GetMixer(), m_MIDIDeviceManager);
		std::string playerName = "Player " + std::to_string(++m_NumPlayback);
		playback->SetName(playerName);
		m_Entities.push_back(playback);

		m_Audio->Start();
	}

	void MainLayer::AddRecordingTrack()
	{
		if (m_NumRecorders >= MaxRecordingDevices)
			return;
		m_Audio->Stop();
		m_RecorderMgr->Stop();

		auto recordingTrack = std::make_shared<RecordingTrack>();
		recordingTrack->Add(m_PlayerMgr, m_RecorderMgr, m_AudioRenderer->GetMixer(), m_MIDIDeviceManager);
		std::string trackName = "Recording " + std::to_string(++m_NumRecorders);
		recordingTrack->SetName(trackName);
		m_Entities.push_back(recordingTrack);

		m_Audio->Start();
	}

	void MainLayer::DeleteEntity()
	{
		m_Audio->Stop();
		m_PlayerMgr->StopAll();
		m_RecorderMgr->Stop();
		m_MIDIDeviceManager->StopDevice();

		auto id = m_EtyToDelete->GetID();
		for (auto itr = m_Entities.begin(); itr != m_Entities.end(); itr++)
		{
			if ((*itr)->GetID() == id)
			{
				m_Entities.erase(itr);
				break;
			}
		}

		m_EtyToDelete->Delete(m_PlayerMgr, m_RecorderMgr, m_AudioRenderer->GetMixer(), m_MIDIDeviceManager);

		switch (m_EtyToDelete->GetType())
		{
		case EntityType::none:
		{
			break;
		}
		case EntityType::synth:
		{
			m_NumSynths--;
			break;
		}
		case EntityType::sampler:
		{
			m_NumSamplers--;
			break;
		}
		case EntityType::playback:
		{
			m_NumPlayback--;
			break;
		}
		case EntityType::recording:
		{
			m_NumRecorders--;
			break;
		}
		default:
			break;
		}

		m_Audio->Start();
		m_EtyToDelete = nullptr;
	}

	bool MainLayer::OnKeyEvent(BackBeat::KeyPressedEvent& event)
	{
		if (event.isRepeat())
		{
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

	unsigned int MainLayer::SetCanvasColors()
	{
		unsigned int count = 0;

		// Main background
		ImGui::PushStyleColor(ImGuiCol_WindowBg, IM_COL32(147, 157, 169, 255)); count++;
		ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(46, 46, 46, 255)); count++;

		// Table colors
		ImGui::PushStyleColor(ImGuiCol_TableRowBg, IM_COL32(147, 157, 169, 255)); count++;
		ImGui::PushStyleColor(ImGuiCol_TableRowBgAlt, IM_COL32(147, 157, 169, 255)); count++;
		ImGui::PushStyleColor(ImGuiCol_TableBorderStrong, IM_COL32(72, 72, 72, 255)); count++;
		ImGui::PushStyleColor(ImGuiCol_TableBorderLight, IM_COL32(72, 72, 72, 255)); count++;

		return count;
	}

	unsigned int MainLayer::SetMainColors()
	{
		unsigned int count = 0;

		ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 255, 255, 255)); count++;
		ImGui::PushStyleColor(ImGuiCol_MenuBarBg, IM_COL32(189, 197, 206, 255)); count++;
		ImGui::PushStyleColor(ImGuiCol_WindowBg, IM_COL32(189, 197, 206, 255)); count++;

		return count;
	}

}