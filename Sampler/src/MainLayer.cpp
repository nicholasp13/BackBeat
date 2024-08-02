#include "MainLayer.h"

// TODO: Put all Sampler objects into a Sampler namespace for clarity, consistent style, and organization

	MainLayer::MainLayer(BackBeat::Window* window)
		: Layer("MainLayer"), m_Window(window), m_NumMIDIDevices(0)
	{
	}

	MainLayer::~MainLayer()
	{

	}

	void MainLayer::OnAttach()
	{
		auto mixer = m_AudioRenderer.GetMixer();
		auto recorderMgr = m_Recorder.GetRecorderMgr();
		auto synthProc = m_Synth.GetSynthProc();
		auto synthID = synthProc->GetID();

		// Prepare Mixer
		mixer->PushProcessor(synthProc);
		mixer->PushProcessor(m_Player.GetProc());
		mixer->PushProcessor(m_SamplerController.GetSamplerGetProcessor());
		mixer->PushProcessor(m_SamplerController.GetTrackGetProcessor());
		mixer->AddRecordingManager(recorderMgr);
		m_AudioRenderer.Start();

		// Prepare MIDI devices
		m_MIDIDeviceManager.PushOutput(m_Synth.GetMIDIInput());
		m_MIDIDeviceManager.PushOutput(m_SamplerController.GetMIDIInput());
		m_NumMIDIDevices = m_MIDIDeviceManager.GetNumDevices();
		for (unsigned int i = 0; i < m_NumMIDIDevices; i++) {
			m_DeviceNames.push_back(m_MIDIDeviceManager.GetDeviceName(i));
		}

		// Prepare Recorders
		m_Recorder.SetSynthID(synthID);
		recorderMgr->AddRecorder(synthID, synthProc->GetProperties());
	}

	void MainLayer::OnDetach()
	{
		m_SamplerController.Close();
		m_Synth.Close();
		m_Player.Close();
		m_Recorder.Close();
		m_AudioRenderer.Stop();
		m_MIDIDeviceManager.CloseAll();
	}
	
	void MainLayer::OnUpdate()
	{
		m_SamplerController.Update();
		m_Synth.Update();
		m_Player.Update();
		m_Recorder.Update();
	}

	// TODO: Add checks to see if any of the objects set event.Handled = true
	void MainLayer::OnEvent(BackBeat::Event& event) 
	{
		// NOTE: This is a set priority list for controls. There are no repeated keys so this does not caause any problems
		//       but future implementations might want to have the priority set by time opened newest to oldest or have
		//       MainLayer keep track and set all key bindings (the latter seams easier and more useful in allowing for custom
		//       keys).
		if (m_SamplerController.IsOpen())
			m_SamplerController.OnEvent(event);
		if (m_Synth.IsOpen())
			m_Synth.OnEvent(event);
		if (m_Player.IsOpen())
			m_Player.OnEvent(event);
		if (m_Recorder.IsOpen())
			m_Recorder.OnEvent(event);

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
				if (ImGui::BeginMenu("Menu"))
				{
					// MIDIDevices
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
							bool s_DeviceOpen = m_MIDIDeviceManager.IsOpen(i);

							if (ImGui::BeginMenu(m_DeviceNames[i].c_str()))
							{
								if (ImGui::MenuItem("Set Active", "", &s_DeviceOpen))
								{
									if (s_DeviceOpen)
									{
										m_MIDIDeviceManager.OpenDevice(i);
										m_MIDIDeviceManager.RunDevice(i);
									}
									else
									{
										m_MIDIDeviceManager.StopDevice();
										m_MIDIDeviceManager.CloseDevice(i);
									}
								}
								
								ImGui::EndMenu();
							}
						}
						
						ImGui::EndMenu();
					}
					
					ImGui::EndMenu();
				}

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

				if (ImGui::BeginMenu("Sampler"))
				{
					if (ImGui::MenuItem("Open"))
					{
						m_SamplerController.Open();
					}
					ImGui::EndMenu();
				}

				if (ImGui::BeginMenu("Recorder"))
				{
					if (ImGui::MenuItem("Open"))
					{
						m_Recorder.Open();
					}
					ImGui::EndMenu();
				}

				ImGui::EndMenuBar();
			}

			ImGui::End();
		}

		m_SamplerController.ImGuiRender();
		m_Synth.ImGuiRender();
		m_Player.ImGuiRender();
		m_Recorder.ImGuiRender();
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