// TODO: - Create editable timeline for entity/track objects
//       - Ask user if they want to save current project if they close window

// NOTE: BUG - Added m_Audio->Start() and m_Audio->Stop() during adding and deleting entities
//       but when deserializing multiple entities it caused the AudioThread to block the 
//       main thread as it was waiting for AudioThread to end but it never does. This also happens
//       if the Start and Stop calls are called quickly between each other. I commented out the function
//       calls and have experience no bugs so far. Delete comments if no harm seems to be done with 
//       not stopping and starting the thread when adding BackBeat audio objects.

#include "MainLayer.h"
namespace Exampler {

	MainLayer::MainLayer(BackBeat::Window* window, BackBeat::AudioSystem* audio)
		: 
		Layer("MainLayer"), 
		m_NewPopupOpen(false),
		m_SaveAsPopupOpen(false),
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
		m_NumMIDIDevices = m_MIDIDeviceManager->GetNumDevices();
		for (unsigned int i = 0; i < m_NumMIDIDevices; i++) 
		{
			m_DeviceNames.push_back(m_MIDIDeviceManager->GetDeviceName(i));
		}

		if (!m_FileMgr.SetWorkingDirectory(m_AppFileDir))
			BB_CLIENT_ERROR("NOT A VALID WORKING DIRECTORY: {0}", m_AppFileDir.c_str());

		m_ProjectNames = m_FileMgr.GetSubDirNames();

		// FOR TESTING
#if FALSE
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
		if (m_State == AppState::Start)
			RenderStartup();

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

			if (m_State == AppState::Start)
				windowFlags |= ImGuiWindowFlags_NoInputs;

			ImGui::Begin("Background", &open, windowFlags);

			RenderMenubar();
			RenderCanvas();
			RenderAudioVisualizer();
			RenderMgrs();
			RenderPopups();

			ImGui::End();
		}

		ImGui::PopStyleColor(count);
	}

	void MainLayer::RenderStartup()
	{
		// ImGui::ShowDemoWindow();
		static int selected = -1;

		unsigned int width = m_Window->GetWidth();
		unsigned int height = m_Window->GetHeight();
		float startWidth = 300.0f;
		float startHeight = 500.0f;
		float startX = (float)width / 2.0f - startWidth / 2.0f;
		float startY = (float)height / 2.0f - startHeight / 2.0f;

		ImGui::SetNextWindowSize(ImVec2(startWidth, startHeight));
		ImGui::SetNextWindowPos(ImVec2(startX, startY), (ImGuiCond)0);

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

			// NOTE: Uses canvas colors for now as a placeholder
			unsigned int cCount = SetCanvasColors();

			ImGui::BeginChild("Projects", ImVec2(startWidth - wBorder, startHeight - hBorder), false, childFlags);
			
			for (int i = 0; i < m_ProjectNames.size(); i++)
			{
				if (ImGui::Selectable(m_ProjectNames[i].c_str(), selected == i, ImGuiSelectableFlags_AllowDoubleClick))
				{
					selected = i;
					if (ImGui::IsMouseDoubleClicked(0))
						LoadProject(m_ProjectNames[selected]);
				}
			}

			ImGui::EndChild();

			ImGui::PopStyleColor(cCount);
		}

		ImGui::BeginDisabled(selected == -1);
		if (ImGui::Button("Load"))
			LoadProject(m_ProjectNames[selected]);
		ImGui::EndDisabled();

		if (ImGui::Button("New"))
			NewProject();

		ImGui::End();
	}

	void MainLayer::RenderMenubar()
	{
		if (ImGui::BeginMenuBar())
		{
			// TODO: Finish
			// Project menu
			if (ImGui::BeginMenu("File"))
			{
				BackBeat::ProjectConfig config = m_ActiveProject->GetConfig();

				// See RenderPopups() to find implementation of how "New" works
				if (ImGui::MenuItem("New"))
				{
					m_NewPopupOpen = true;
				}

				// Should either be a drop down menu with all projects listed
				// OR open file dialog to open a project XML
				if (ImGui::MenuItem("Open"))
				{

				}

				const auto untitled = std::string("Untitled");
				// Should be disabled when Active Project has not been saved yet
				ImGui::BeginDisabled(config.name == untitled);
				if (ImGui::MenuItem("Save"))
				{
					SaveProject();
				}
				ImGui::EndDisabled();

				// See RenderPopups() to find implementation of how "Save As" works
				if (ImGui::MenuItem("Save As"))
				{
					m_SaveAsPopupOpen = true;
				}

				ImGui::EndMenu();
			}
				
			// TODO: Add way to add tracks through this menu
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

		if (m_State == AppState::Start)
			childFlags |= ImGuiWindowFlags_NoInputs;

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

	// TODO: Hide this initially
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
			std::string name = "Channel " + std::to_string(i + 1);
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
		// Renaming Entity Popup
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

		// Deleting Entity Popup
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

		// TODO: FINISH
		// New Project Popup
		if (m_NewPopupOpen)
			ImGui::OpenPopup("NewPopupID");
		if (ImGui::BeginPopup("NewPopupID"))
		{
			ImGui::Text("WORK IN PROGRESS");
			m_NewPopupOpen = false;

			ImGui::EndPopup();
		}

		// SavingAs Project Popup
		if (m_SaveAsPopupOpen)
			ImGui::OpenPopup("SaveAsPopupID");
		if (ImGui::BeginPopup("SaveAsPopupID"))
		{
			BackBeat::ProjectConfig config = m_ActiveProject->GetConfig();
			std::string oldName = config.name;
			char changeName[128] = {};
			bool opened = false;

			ImGui::InputTextWithHint("Input New Project Name", oldName.c_str(), changeName,
				IM_ARRAYSIZE(changeName));
			if (ImGui::IsItemDeactivated())
			{
				auto newName = std::string(changeName);
				if (newName.size() > 0 && newName != oldName)
				{
					// TODO: Add user ability to overwrite
					if (m_FileMgr.Exists(newName))
					{
						BB_CLIENT_ERROR("Project name taken");
					}
					else
					{

						m_ActiveProject = BackBeat::Project::New();
						m_ActiveProject->GetConfig().app = "Exampler";
						m_ActiveProject->GetConfig().name = newName;

						auto projectDirPath = m_FileMgr.CreateSubDirectory(newName);
						m_ActiveProject->GetConfig().projectDirectoryPath = projectDirPath;

						auto xmlFilePath = projectDirPath + newName + ".xml";
						m_ActiveProject->GetConfig().xmlFilePath = xmlFilePath;

						SaveProject();
					}
				}
				ImGui::CloseCurrentPopup();
			}
			m_SaveAsPopupOpen = false;

			ImGui::EndPopup();
		}
	}

	void MainLayer::AddSynth()
	{
		if (m_NumSynths >= MaxSynths)
			return;
		// m_Audio->Stop();

		auto synth = std::make_shared<Synthesizer>();
		synth->Add(m_PlayerMgr, m_RecorderMgr, m_AudioRenderer->GetMixer(), m_MIDIDeviceManager);
		std::string synthName = "Synth " + std::to_string(++m_NumSynths);
		synth->SetName(synthName);
		m_Entities.push_back(synth);
		
		// m_Audio->Start();
	}

	void MainLayer::AddSampler()
	{
		if (m_NumSamplers >= MaxSamplers)
			return;
		// m_Audio->Stop();

		auto sampler = std::make_shared<Sampler>();
		sampler->Add(m_PlayerMgr, m_RecorderMgr, m_AudioRenderer->GetMixer(), m_MIDIDeviceManager);
		std::string samplerName = "Sampler " + std::to_string(++m_NumSamplers);
		sampler->SetName(samplerName);
		m_Entities.push_back(sampler);

		// m_Audio->Start();
	}

	void MainLayer::AddPlaybackTrack()
	{
		if (m_NumPlayback >= MaxPlayback)
			return;
		// m_Audio->Stop();

		auto playback = std::make_shared<PlaybackTrack>();
		playback->Add(m_PlayerMgr, m_RecorderMgr, m_AudioRenderer->GetMixer(), m_MIDIDeviceManager);
		std::string playerName = "Player " + std::to_string(++m_NumPlayback);
		playback->SetName(playerName);
		m_Entities.push_back(playback);

		// m_Audio->Start();
	}

	void MainLayer::AddRecordingTrack()
	{
		if (m_NumRecorders >= MaxRecordingDevices)
			return;
		// m_Audio->Stop();
		m_RecorderMgr->Stop();

		auto recordingTrack = std::make_shared<RecordingTrack>();
		recordingTrack->Add(m_PlayerMgr, m_RecorderMgr, m_AudioRenderer->GetMixer(), m_MIDIDeviceManager);
		std::string trackName = "Recording " + std::to_string(++m_NumRecorders);
		recordingTrack->SetName(trackName);
		m_Entities.push_back(recordingTrack);

		// m_Audio->Start();
	}

	void MainLayer::DeleteEntity()
	{
		// m_Audio->Stop();
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

		// m_Audio->Start();
		m_EtyToDelete = nullptr;
	}

	bool MainLayer::LoadProject(std::string project)
	{
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

		// TODO: DELETE WHEN DONE TESTING DESERIALIZATION
		/**
		// auto projectXMLName = project + ".xml";
		auto projectXMLName = project + ".txt"; // TODO: DELETE, For testing
		auto projectXMLPath = projectMgr.GetFilePath(projectXMLName);

		if (projectXMLPath.empty())
		{
			BB_CLIENT_ERROR("NOT A VALID PROJECT XML FILE PATH! FAILED TO FIND! \nFILE NAME: {0}", 
				projectXMLName.c_str());
			NewProject();
			return false;
		}

		m_ActiveProject = BackBeat::Project::Load(projectXMLPath);
		/**/

		std::string testPath = "C:\\Dev\\Testing\\BackBeat\\test.xml"; // TODO: DELETE AND USE FILEPATH
		m_ActiveProject = BackBeat::Project::Load(testPath); // TODO: DELETE AND USE FILEPATH

		if (!m_ActiveProject)
		{
			BB_CLIENT_ERROR("NOT A VALID PROJECT XML FILE! FAILED TO DESERIALIZE!");
			// TODO: DELETE WHEN DONE TESTING DESERIALIZATION
			// BB_CLIENT_ERROR("NOT A VALID PROJECT XML FILE! FAILED TO DESERIALIZE! \nFILE NAME: {0}\nFILE PATH: {1}",
			//	projectXMLName.c_str(), projectXMLPath.c_str());
			NewProject();
			return false;
		}
		
		Deserialize(testPath);

		m_State = AppState::Play;
		return true;
	}

	void MainLayer::NewProject()
	{
		m_ActiveProject = BackBeat::Project::New();
		m_ActiveProject->GetConfig().app = "Exampler";

		switch (m_State)
		{
		
		case AppState::Start:
		{
			m_State = AppState::Play;
			break;
		}
		
		case AppState::Play:
		{
			// Should clear all entities here

			break;
		}

		default:
		{
			break;
		}


		}
	}

	void MainLayer::SaveProject()
	{
		m_ActiveProject->GetConfig().objectList.clear();
		for (auto itr = m_Entities.begin(); itr != m_Entities.end(); itr++)
		{
			m_ActiveProject->GetConfig().objectList.push_back(*itr);
		}

		BackBeat::Project::SaveActive(m_ActiveProject->GetConfig().xmlFilePath);
	}

	// TODO: Implement after all Entity parameters are done
	void MainLayer::Serialize(std::string filePath)
	{
		// NOTE: Empty as BackBeat::Project::SaveActive handles all serialization but will be
		//       filled as needed to serialize Exampler specific stuff like which tracks are
		//       being recorded and where
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

		for (pugi::xml_node_iterator itr = objectsNode.begin(); itr != objectsNode.end(); itr++)
		{
			auto entityType = itr->name();
			if (strcmp(entityType, "Synthesizer") == 0)
			{
				AddSynth();
				m_Entities.back()->ReadObject(&(*itr));
			}
			else if (strcmp(entityType, "Playback") == 0)
			{
				AddPlaybackTrack();
				m_Entities.back()->ReadObject(&(*itr));
			}
			else
			{
				BB_CLIENT_ERROR("UNRECOGNIZED TRACK TYPE IN XML: {0}", entityType);
			}
			// TODO: Add other entities as needed
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