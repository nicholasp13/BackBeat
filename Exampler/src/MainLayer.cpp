// TODO: - Allow for user input and editing in ImGuiTimeline
//       - Add the ability to save specific configs from certain entities i.e. Synth's to a config xml file
//       - Allow user to change the audio input channel for RecordingTrack and between MONO and STEREO andd
//           allow for user to select where the recording starts in the file

#include "MainLayer.h"
namespace Exampler {

	MainLayer::MainLayer(BackBeat::Window* window, BackBeat::AudioSystem* audio)
		: 
		Layer("MainLayer"), 
		m_NewPopupOpen(false),
		m_SaveAsPopupOpen(false),
		m_OpenAudioVisualizer(false),
		m_NumMIDIDevices(0),
		m_NumSynths(0),
		m_NumSamplers(0),
		m_NumPlayback(0),
		m_NumRecorders(0),
		m_AppFileDir(BackBeat::FileSystem::GetAppDir()),
		m_EtyToRename(nullptr),
		m_EtyToDelete(nullptr),
		m_Window(window),
		m_Audio(audio),
		m_PlayerMgr(audio->GetPlayerManager()),
		m_RecorderMgr(audio->GetRecorderManager()),
		m_AudioRenderer(audio->GetRenderer()),
		m_MIDIDeviceManager(audio->GetMIDIDeviceManager()),
		m_Visualizer(audio->GetVisualizer()),
		m_ActiveProject(nullptr),
		m_State(AppState::Start)
	{

	}

	MainLayer::~MainLayer()
	{
		
	}

	void MainLayer::OnAttach()
	{
		m_Canvas.Init(m_Audio->GetProps(), m_PlayerMgr, m_RecorderMgr);

		m_NumMIDIDevices = m_MIDIDeviceManager->GetNumDevices();
		for (unsigned int i = 0; i < m_NumMIDIDevices; i++) 
		{
			m_DeviceNames.push_back(m_MIDIDeviceManager->GetDeviceName(i));
		}

		if (!m_FileMgr.SetWorkingDirectory(m_AppFileDir))
			BB_CLIENT_ERROR("NOT A VALID WORKING DIRECTORY: {0}", m_AppFileDir.c_str());

		// FOR TESTING
#if FALSE
		m_Entities.push_back(std::make_shared<Dummy>());
#endif
	}

	void MainLayer::OnDetach()
	{
		ClearEntities();
	}

	void MainLayer::OnUpdate()
	{

		switch (m_State)
		{

		case AppState::Start:
		{
			break;
		}

		case AppState::Play:
		{
			for (auto itr = m_Entities.begin(); itr != m_Entities.end(); itr++)
			{
				(*itr)->Update();
			}
			m_Visualizer->Update();
			break;
		}

		}
	}

	void MainLayer::OnEvent(BackBeat::Event& event)
	{
		switch (m_State)
		{

		case AppState::Start:
		{
			break;
		}

		case AppState::Play:
		{
			for (auto itr = m_Entities.begin(); itr != m_Entities.end(); itr++)
			{
				(*itr)->OnEvent(event);
			}
			break;
		}

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

		// Renders Start window
		if (m_State == AppState::Start || m_State == AppState::SelectProject)
			RenderProjectMgr();

		// Render background
		{
			ImGui::SetNextWindowSize(ImVec2((float)width, (float)height));
			ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f), (ImGuiCond)0);
			// Background flags
			ImGuiWindowFlags windowFlags = 0;
			windowFlags |= ImGuiWindowFlags_NoTitleBar;
			windowFlags |= ImGuiWindowFlags_NoMove;
			windowFlags |= ImGuiWindowFlags_NoCollapse;
			windowFlags |= ImGuiWindowFlags_NoResize;
			windowFlags |= ImGuiWindowFlags_MenuBar;
			windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
			bool open;

			if (m_State != AppState::Play)
				windowFlags |= ImGuiWindowFlags_NoInputs;

			ImGui::Begin("Background", &open, windowFlags);

			RenderMenubar();
			RenderCanvas();
			RenderMgrs();
			RenderAudioVisualizer();
			RenderMenubarPopups();

			ImGui::End();
		}

		ImGui::PopStyleColor(count);
	}

	void MainLayer::RenderProjectMgr()
	{
		static int selected = -1;
		static bool warning = false;

		m_ProjectNames = m_FileMgr.GetSubDirNames();

		unsigned int width = m_Window->GetWidth();
		unsigned int height = m_Window->GetHeight();
		float startWidth = 300.0f;
		float startHeight = 500.0f;

		ImGui::SetNextWindowSize(ImVec2(startWidth, startHeight));
		ImGui::SetNextWindowPos(Center(startWidth, startHeight), (ImGuiCond)0);

		ImGuiWindowFlags windowFlags = 0;
		windowFlags |= ImGuiWindowFlags_NoTitleBar;
		windowFlags |= ImGuiWindowFlags_NoMove;
		windowFlags |= ImGuiWindowFlags_NoCollapse;
		windowFlags |= ImGuiWindowFlags_NoResize;
		bool open;

		ImGui::Begin("Startup", &open, windowFlags);

		ImGui::SeparatorText("Projects");

		// Renders projects selector
		{
			const float wBorder = 20;
			const float hBorder = 100;
			ImGuiWindowFlags childFlags = 0;
			childFlags |= ImGuiWindowFlags_NoTitleBar;
			childFlags |= ImGuiWindowFlags_NoMove;
			childFlags |= ImGuiWindowFlags_NoCollapse;
			childFlags |= ImGuiWindowFlags_NoResize;
			childFlags |= ImGuiWindowFlags_AlwaysVerticalScrollbar;

			unsigned int cCount = SetProjectMgrColors();

			ImGui::BeginChild("Projects", ImVec2(startWidth - wBorder, startHeight - hBorder), false, childFlags);
			
			for (int i = 0; i < m_ProjectNames.size(); i++)
			{
				if (ImGui::Selectable(m_ProjectNames[i].c_str(), selected == i, ImGuiSelectableFlags_AllowDoubleClick))
				{
					selected = i;
					if (ImGui::IsMouseDoubleClicked(0))
					{
						if (m_State == AppState::Start)
							LoadProject(m_ProjectNames[selected]);
						else
							warning = true;
					}
				}
			}

			ImGui::EndChild();

			ImGui::PopStyleColor(cCount);
		}

		ImGui::BeginDisabled(selected == -1);

		// Renders Load button and popup
		{
			if (ImGui::Button("Load"))
			{
				if (m_State == AppState::Start)
					LoadProject(m_ProjectNames[selected]);
				else
					warning = true;
			}

			const float width = 500.0f;
			const float height = 100.0f;
			ImGui::SetNextWindowSize(ImVec2(width, height));

			if (warning)
				ImGui::OpenPopup("##LoadWarningProjectPopupID");
			if (ImGui::BeginPopupModal("##LoadWarningProjectPopupID", &warning, ImGuiWindowFlags_NoResize))
			{
				std::string activeProjectName = m_ActiveProject->GetConfig().name;
				if (activeProjectName != "Untitled")
				{
					ImGui::Text("Save changes to"); ImGui::SameLine();
					ImGui::Text(activeProjectName.c_str()); ImGui::SameLine();
					ImGui::Text("before loading another project?");
					ImGui::Text(" ");

					if (ImGui::Button("Save"))
					{
						SaveProject();
						LoadProject(m_ProjectNames[selected]);
						warning = false;
						ImGui::CloseCurrentPopup();
					}
					ImGui::SameLine();

					if (ImGui::Button("Don't Save"))
					{
						LoadProject(m_ProjectNames[selected]);
						warning = false;
						ImGui::CloseCurrentPopup();
					}
					ImGui::SameLine();
				}
				else
				{
					ImGui::Text("Current project is unsaved.");
					ImGui::Text("Do you wish to continue to load a different project?");

					if (ImGui::Button("Continue"))
					{
						LoadProject(m_ProjectNames[selected]);
						warning = false;
						ImGui::CloseCurrentPopup();
					}
					ImGui::SameLine();
				}

				if (ImGui::Button("Cancel"))
				{
					warning = false;
					ImGui::CloseCurrentPopup();
				}

				ImGui::EndPopup();
			}
		}

		ImGui::SameLine(); ImGui::Text("                         "); ImGui::SameLine();

		// Delete buttons and Delete popup
		{
			static bool toDelete = true;
			if (ImGui::Button("Delete"))
			{
				toDelete = true;
				ImGui::OpenPopup("##DeleteProjectPopupID");
			}

			const float width = 500.0f;
			const float height = 100.0f;
			ImGui::SetNextWindowSize(ImVec2(width, height));

			if (ImGui::BeginPopupModal("##DeleteProjectPopupID", &toDelete, ImGuiWindowFlags_NoResize))
			{
				ImGui::Text("Are you sure you want to delete"); ImGui::SameLine();
				ImGui::Text(m_ProjectNames[selected].c_str()); ImGui::SameLine();
				ImGui::Text("project?"); ImGui::NewLine();

				if (ImGui::Button("Yes"))
				{
					std::string selecetedProject = m_ProjectNames[selected];
					if (m_ActiveProject)
					{
						if (selecetedProject == m_ActiveProject->GetConfig().name)
						{
							NewProject();
							m_State = AppState::SelectProject;
						}
					}

					DeleteProject(selecetedProject);

					ImGui::CloseCurrentPopup();
				} ImGui::SameLine();

				if (ImGui::Button("No"))
				{
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndPopup();
			}

		}

		ImGui::EndDisabled();

		if (ImGui::Button("New"))
			NewProject();

		if (m_State == AppState::SelectProject)
		{
			ImGui::SameLine(); ImGui::Text("                          "); ImGui::SameLine();

			if (ImGui::Button("Cancel"))
			{
				m_State = AppState::Play;
			}
		}

		ImGui::End();
	}

	void MainLayer::RenderMenubar()
	{
		if (ImGui::BeginMenuBar())
		{

			// Project menu
			if (ImGui::BeginMenu("File"))
			{
				BackBeat::ProjectConfig config = m_ActiveProject->GetConfig();

				// See RenderMenubarPopups() to find implementation of how "New" works
				if (ImGui::MenuItem("New"))
				{
					m_NewPopupOpen = true;
				}

				if (ImGui::MenuItem("Open"))
				{
					m_State = AppState::SelectProject;
				}

				const auto untitled = std::string("Untitled");
				ImGui::BeginDisabled(config.name == untitled);
				if (ImGui::MenuItem("Save"))
				{
					SaveProject();
				}
				ImGui::EndDisabled();

				// See RenderMenubarPopups() to find implementation of how "Save As" works
				if (ImGui::MenuItem("Save As"))
				{
					m_SaveAsPopupOpen = true;
				}

				ImGui::EndMenu();
			}
				
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
				
				ImGui::Separator();

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

				ImGui::EndMenu();
			}

			// MIDI Devices menu
			if (ImGui::BeginMenu("Devices"))
			{
				if (ImGui::BeginMenu("MIDI Devices"))
				{
					if (m_NumMIDIDevices == 0)
					{
						ImGui::BeginDisabled();
						if (ImGui::MenuItem("No devices detected"))
						{
						}
						ImGui::EndDisabled();
					}

					for (unsigned int i = 0; i < m_NumMIDIDevices; i++) {

						// TODO: Test multiple MIDI devices and if MIDI devices change number
						bool deviceRunning = m_MIDIDeviceManager->IsRunning(i);

						if (ImGui::BeginMenu(m_DeviceNames[i].c_str()))
						{
							if (ImGui::MenuItem("Set Active", "", &deviceRunning))
							{
								if (deviceRunning)
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
		const unsigned int wBorder = 20;
		const unsigned int hBorder = 250;

		m_Canvas.Render((float)(width - wBorder), (float)(height - hBorder), m_State == AppState::Play);
	}

	// BUG: When spamming the on and off button. It may cause the visuals to glitch (mostly show the data when it should have been flushed)
	void MainLayer::RenderAudioVisualizer()
	{
		ImGui::Spacing();

		if (!m_OpenAudioVisualizer)
		{
			if (ImGui::Button("Open Audio Visualizer"))
			{
				m_OpenAudioVisualizer = true;
			}

			return;
		}

		if (ImGui::Button("Close Audio Visualizer"))
		{
			m_OpenAudioVisualizer = false;;
			m_Visualizer->Off();
			return;
		}

		ImGui::Spacing(); ImGui::Separator(); ImGui::Spacing();

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
			std::string name = "Channel " + std::to_string(i + 1);
			ImGui::PlotLines(name.c_str(), m_Visualizer->GetChannelBuffer(i), bufferSize, 1, "", 
				visualMax * -1, visualMax, ImVec2(m_Window->GetWidth() - 200.0f, 60.0f));
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

	void MainLayer::RenderMenubarPopups()
	{
		// Renaming Entity Popup
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
		}

		// Deleting Entity Popup
		{
			if (m_EtyToDelete)
				ImGui::OpenPopup("DeletePopupID");
			if (ImGui::BeginPopup("DeletePopupID"))
			{
				ImGui::Text("Are you sure you want to delete"); ImGui::SameLine();
				ImGui::Text(m_EtyToDelete->GetName().c_str()); ImGui::SameLine();
				ImGui::Text("?");

				if (ImGui::Button("Yes"))
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

		// New Project Popup
		{
			const float width = 500.0f;
			const float height = 100.0f;
			ImGui::SetNextWindowSize(ImVec2(width, height));

			if (m_NewPopupOpen)
				ImGui::OpenPopup("##NewPopupID");
			if (ImGui::BeginPopupModal("##NewPopupID", &m_NewPopupOpen, ImGuiWindowFlags_NoResize))
			{
				std::string activeProjectName = m_ActiveProject->GetConfig().name;
				if (activeProjectName != "Untitled")
				{
					ImGui::Text("Save changes to"); ImGui::SameLine();
					ImGui::Text(activeProjectName.c_str()); ImGui::SameLine();
					ImGui::Text("before closing?");

					if (ImGui::Button("Save"))
					{
						SaveProject();
						NewProject();
						m_NewPopupOpen = false;
					}
					ImGui::SameLine();

					if (ImGui::Button("Don't Save"))
					{
						NewProject();
						m_NewPopupOpen = false;
						ImGui::CloseCurrentPopup();
					}
					ImGui::SameLine();

				}
				else
				{
					ImGui::Text("Current project is unsaved.");
					ImGui::Text("Do you wish to continue to a new project?");

					if (ImGui::Button("Continue"))
					{
						NewProject();
						m_NewPopupOpen = false;
						ImGui::CloseCurrentPopup();
					}
					ImGui::SameLine();
				}

				if (ImGui::Button("Cancel"))
				{
					m_NewPopupOpen = false;
					ImGui::CloseCurrentPopup();
				}

				ImGui::EndPopup();
			}
		}

		// SavingAs Project Popup
		{
			const float width = 500.0f;
			const float height = 125.0f;
			ImGui::SetNextWindowSize(ImVec2(width, height));

			if (m_SaveAsPopupOpen)
				ImGui::OpenPopup("##SaveAsPopupID");
			if (ImGui::BeginPopupModal("##SaveAsPopupID", &m_SaveAsPopupOpen, ImGuiWindowFlags_NoResize))
			{
				const std::string untitled = "Untitled";
				static bool valid = true;

				BackBeat::ProjectConfig config = m_ActiveProject->GetConfig();
				std::string oldName = config.name;
				const int nameSize = 128;
				static char changeName[nameSize] = {};

				ImGui::Text("Enter name");
				ImGui::InputTextWithHint("##ProjectInputText", untitled.c_str(), changeName,
					IM_ARRAYSIZE(changeName));

				auto newName = std::string(changeName);

				if (m_FileMgr.Exists(newName) && newName.size() > 0)
				{
					ImGui::Text("Invalid! Name taken!");
					valid = false;
				}
				else if (newName == untitled)
				{
					ImGui::Text("Invalid name!");
					valid = false;
				}
				else
				{
					ImGui::Text(" "); 
					valid = true;
				}

				ImGui::BeginDisabled(!valid);
				if (ImGui::Button("Save"))
				{
					SaveAsProject(newName);
					m_SaveAsPopupOpen = false;
					memset(changeName, 0, nameSize * sizeof(char));
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndDisabled();

				ImGui::SameLine();
				if (ImGui::Button("Cancel"))
				{
					m_SaveAsPopupOpen = false;
					memset(changeName, 0, nameSize * sizeof(char));
					ImGui::CloseCurrentPopup();
				}

				ImGui::EndPopup();
			}
		}

	}

	void MainLayer::AddSynth()
	{
		if (m_NumSynths >= MaxSynths)
			return;

		auto synth = std::make_shared<Synthesizer>();
		synth->Add(m_PlayerMgr, m_RecorderMgr, m_AudioRenderer->GetMixer(), m_MIDIDeviceManager);
		std::string synthName = "Synth " + std::to_string(++m_NumSynths);
		synth->SetName(synthName);
		m_Entities.push_back(synth);
		m_Canvas.AddEntity(synth);
	}

	void MainLayer::AddSampler()
	{
		if (m_NumSamplers >= MaxSamplers)
			return;

		auto sampler = std::make_shared<Sampler>();
		sampler->Add(m_PlayerMgr, m_RecorderMgr, m_AudioRenderer->GetMixer(), m_MIDIDeviceManager);
		std::string samplerName = "Sampler " + std::to_string(++m_NumSamplers);
		sampler->SetName(samplerName);
		m_Entities.push_back(sampler);
		m_Canvas.AddEntity(sampler);
	}

	void MainLayer::AddPlaybackTrack()
	{
		if (m_NumPlayback >= MaxPlayback)
			return;

		auto playback = std::make_shared<PlaybackTrack>();
		playback->Add(m_PlayerMgr, m_RecorderMgr, m_AudioRenderer->GetMixer(), m_MIDIDeviceManager);

		// If playback loads incorrectly GetMappedTrack() will return a nullptr
		if (playback->GetMappedTrack())
		{
			std::string playerName = "Playback " + std::to_string(++m_NumPlayback);
			playback->SetName(playerName);
			m_Entities.push_back(playback);
			m_Canvas.AddEntity(playback);
		}
	}

	void MainLayer::AddPlaybackTrack(std::string filePath)
	{
		if (m_NumPlayback >= MaxPlayback)
			return;

		auto playback = std::make_shared<PlaybackTrack>();
		playback->Add(m_PlayerMgr, m_RecorderMgr, m_AudioRenderer->GetMixer(), m_MIDIDeviceManager, filePath);

		// If playback loads incorrectly GetMappedTrack() will return a nullptr
		if (playback->GetMappedTrack())
		{
			std::string playerName = "Playback " + std::to_string(++m_NumPlayback);
			playback->SetName(playerName);
			m_Entities.push_back(playback);
			m_Canvas.AddEntity(playback);
		}
	}

	void MainLayer::AddRecordingTrack()
	{
		if (m_NumRecorders >= MaxRecordingDevices)
			return;

		m_RecorderMgr->Stop();

		auto recordingTrack = std::make_shared<RecordingTrack>();
		recordingTrack->Add(m_PlayerMgr, m_RecorderMgr, m_AudioRenderer->GetMixer(), m_MIDIDeviceManager);
		std::string trackName = "Recording " + std::to_string(++m_NumRecorders);
		recordingTrack->SetName(trackName);
		m_Entities.push_back(recordingTrack);
		m_Canvas.AddEntity(recordingTrack);
	}

	void MainLayer::DeleteEntity()
	{
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

		m_Canvas.DeleteEntity(m_EtyToDelete);

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

		m_EtyToDelete = nullptr;
	}

	void MainLayer::ClearEntities()
	{
		while (!m_Entities.empty())
		{
			m_EtyToDelete = m_Entities.front();
			DeleteEntity();
		}
		// BackBeat::FileSystem::ClearTempDir();
	}

	bool MainLayer::LoadProject(std::string project)
	{
		m_State = AppState::Load;
		// NOTE: Adding "\\" is a current lazy solution but this should happen in BackBeat::FileManager
		auto projectPath = m_FileMgr.GetSubDirPath(project) + "\\";
		// NOTE: May want to change into a class member or use exisitng member BackBeat::FileManager
		auto projectMgr = BackBeat::FileManager();

		if (!projectMgr.SetWorkingDirectory(projectPath))
		{
			BB_CLIENT_ERROR("NOT A VALID WORKING PROJECT DIRECTORY: {0}", project.c_str());
			NewProject();
			return false;
		}

		auto projectXMLName = project + ".xml";
		auto projectXMLPath = projectMgr.GetFilePath(projectXMLName);

		if (projectXMLPath.empty())
		{
			BB_CLIENT_ERROR("NOT A VALID PROJECT XML FILE PATH! FAILED TO FIND! \nFILE NAME: {0}", 
				projectXMLName.c_str());
			NewProject();
			return false;
		}

		m_ActiveProject = BackBeat::Project::Load(projectXMLPath);

		if (!m_ActiveProject)
		{
			BB_CLIENT_ERROR("NOT A VALID PROJECT XML FILE! FAILED TO DESERIALIZE!");
			NewProject();
			return false;
		}
		
		ClearEntities();
		Deserialize(projectXMLPath);

		m_PlayerMgr->ResetAll();
		m_Canvas.Reset();
		m_State = AppState::Play;
		return true;
	}

	void MainLayer::NewProject()
	{
		m_ActiveProject = BackBeat::Project::New();
		m_ActiveProject->GetConfig().app = "Exampler";
		m_ActiveProject->GetConfig().tracksDirectoryPath = BackBeat::FileSystem::GetTempDir();

		switch (m_State)
		{
		
		case AppState::Start:
		{
			m_State = AppState::Play;
			break;
		}
		
		case AppState::Play:
		{
			ClearEntities();
			break;
		}

		case AppState::SelectProject:
		{
			ClearEntities();
			m_State = AppState::Play;
			break;
		}

		default:
		{
			break;
		}

		}

		m_PlayerMgr->ResetAll();
		m_Canvas.Reset();
	}

	void MainLayer::SaveProject()
	{
		m_ActiveProject->GetConfig().objectList.clear();
		for (auto itr = m_Entities.begin(); itr != m_Entities.end(); itr++)
		{
			m_ActiveProject->GetConfig().objectList.push_back(*itr);
		}

		std::string xmlFilePath = m_ActiveProject->GetConfig().xmlFilePath;

		if (BackBeat::Project::SaveActive(xmlFilePath))
			Serialize(xmlFilePath);
	}

	void MainLayer::SaveAsProject(std::string project)
	{
		const std::string untitled = "Untitled";

		if (project == untitled)
		{
			BB_CLIENT_ERROR("Invalid project name");
		}
		else if (m_FileMgr.Exists(project))
		{
			BB_CLIENT_ERROR("Project name taken");
		}
		else
		{
			m_ActiveProject = BackBeat::Project::New();
			m_ActiveProject->GetConfig().app = "Exampler";
			m_ActiveProject->GetConfig().name = project;

			auto projectDirPath = m_FileMgr.CreateSubDirectory(project);
			m_ActiveProject->GetConfig().projectDirectoryPath = projectDirPath;

			auto projectFileMgr = BackBeat::FileManager();
			projectFileMgr.SetWorkingDirectory(projectDirPath);
			std::string trackSubDir = "Tracks";
			m_ActiveProject->GetConfig().tracksDirectoryPath = projectFileMgr.CreateSubDirectory(trackSubDir);

			auto xmlFilePath = projectDirPath + project + ".xml";
			m_ActiveProject->GetConfig().xmlFilePath = xmlFilePath;

			SaveProject();
		}
	}

	void MainLayer::DeleteProject(std::string project)
	{
		m_FileMgr.DeleteSubDirectory(project);
	}

	void MainLayer::Serialize(std::string filePath)
	{
		auto doc = pugi::xml_document();
		pugi::xml_parse_result result = doc.load_file(filePath.c_str());

		if (!result)
		{
			BB_CLIENT_ERROR("ERROR DESERIALIZING XML FILE OBJECTS");
			return;
		}

		auto appNode = doc.first_child();

		// MIDI Devices
		{
			auto midiDevicesNode = appNode.append_child("MIDIDevices");
			
			for (unsigned int i = 0; i < m_NumMIDIDevices; i++)
			{
				auto deviceNode = midiDevicesNode.append_child("Device");

				deviceNode.append_attribute("ID") = i;
				deviceNode.append_attribute("Name") = m_DeviceNames[i];

				auto onNode = deviceNode.append_child("On");
				onNode.append_attribute("Value") = m_MIDIDeviceManager->IsRunning(i);
			}
		}

		if (!doc.save_file(filePath.c_str()))
			BB_CORE_ERROR("ERROR SAVING XML FILE");
	}

	void MainLayer::Deserialize(std::string filePath)
	{
		auto doc = pugi::xml_document();
		pugi::xml_parse_result result = doc.load_file(filePath.c_str());

		if (!result)
		{
			BB_CLIENT_ERROR("ERROR DESERIALIZING XML FILE OBJECTS");
			return;
		}

		auto appNode = doc.first_child();
		auto objectsNode = appNode.child("Objects");

		if (objectsNode.empty())
		{
			BB_CLIENT_ERROR("ERROR DESERIALIZING XML FILE OBJECTS");
			return;
		}

		// Objects/Entities
		{
			size_t numEntities = 0;
			for (pugi::xml_node_iterator itr = objectsNode.begin(); itr != objectsNode.end(); itr++)
			{
				numEntities = m_Entities.size();
				auto entityType = itr->name();
				if (strcmp(entityType, "Synthesizer") == 0)
					AddSynth();
				else if (strcmp(entityType, "Sampler") == 0)
					AddSampler();
				else if (strcmp(entityType, "Playback") == 0)
				{
					std::string filePath = itr->child("File").attribute("Path").as_string();
					if (filePath.empty())
						AddPlaybackTrack();
					else
						AddPlaybackTrack(filePath);
				}
				else if (strcmp(entityType, "Recorder") == 0)
					AddRecordingTrack();
				else
					BB_CLIENT_ERROR("UNRECOGNIZED TRACK TYPE IN XML: {0}", entityType);

				if (numEntities != m_Entities.size())
					m_Entities.back()->ReadObject(&(*itr));
			}
		}

		// MIDI Devices
		{
			auto midiDevicesNode = appNode.child("MIDIDevices");

			unsigned int i = 0;
			for (pugi::xml_node_iterator itr = midiDevicesNode.begin(); itr != midiDevicesNode.end(); itr++)
			{
				if (i >= m_NumMIDIDevices)
					break;

				pugi::xml_node deviceNode = *itr;
				if (deviceNode.attribute("ID").as_uint() != i)
				{
					i++;
					continue;
				}

				if (deviceNode.attribute("Name").as_string() != m_DeviceNames[i])
				{
					i++;
					continue;
				}

				if (deviceNode.child("On").attribute("Value").as_bool())
				{
					if (m_MIDIDeviceManager->OpenDevice(i))
						m_MIDIDeviceManager->RunDevice(i);
				}

				i++;
			}
		}
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

	unsigned int MainLayer::SetMainColors()
	{
		unsigned int count = 0;

		ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 255, 255, 255)); count++;
		ImGui::PushStyleColor(ImGuiCol_MenuBarBg, IM_COL32(189, 197, 206, 255)); count++;
		ImGui::PushStyleColor(ImGuiCol_WindowBg, IM_COL32(189, 197, 206, 255)); count++;

		return count;
	}

	unsigned int MainLayer::SetProjectMgrColors()
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

	ImVec2 MainLayer::Center(float width, float height)
	{
		if (width < 0.0f || height < 0.0f)
			return ImVec2();

		float windowWidth = (float)m_Window->GetWidth();
		float windowHeight = (float)m_Window->GetHeight();

		if (width > windowWidth || height > windowHeight)
			return ImVec2();

		float centerX = windowWidth / 2.0f - width / 2.0f;
		float centerY = windowHeight / 2.0f - height / 2.0f;

		return ImVec2(centerX, centerY);
	}

}