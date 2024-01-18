#include "MainLayer.h"

	MainLayer::MainLayer(BackBeat::Window* window)
		: Layer("MainLayer"), m_Window(window), m_NumMIDIDevices(0)
	{
	}

	MainLayer::~MainLayer()
	{

	}

	void MainLayer::OnAttach()
	{
		m_AudioRenderer.GetMixer()->PushProcessor(m_Synth.GetSynthProc());
		m_AudioRenderer.GetMixer()->PushProcessor(m_Player.GetProc());
		m_AudioRenderer.GetMixer()->PushProcessor(m_SamplerController.GetSamplerGetProcessor());
		m_AudioRenderer.GetMixer()->PushProcessor(m_SamplerController.GetTrackGetProcessor());
		m_AudioRenderer.Start();

		m_MIDIDeviceManager.PushOutput(m_Synth.GetMIDIInput());
		m_MIDIDeviceManager.PushOutput(m_SamplerController.GetMIDIInput());

		m_NumMIDIDevices = m_MIDIDeviceManager.GetNumDevices();
		for (unsigned int i = 0; i < m_NumMIDIDevices; i++) {
			m_DeviceNames.push_back(m_MIDIDeviceManager.GetDeviceName(i));
		}
	}

	void MainLayer::OnDetach()
	{
		m_SamplerController.Close();
		m_Synth.Close();
		m_Player.Close();
		m_AudioRenderer.Stop();
		m_MIDIDeviceManager.CloseAll();
	}
	
	void MainLayer::OnUpdate()
	{
		m_SamplerController.Update();
		m_Synth.Update();
		m_Player.Update();
	}

	void MainLayer::OnEvent(BackBeat::Event& event) 
	{
		if (m_SamplerController.IsOpen())
			m_SamplerController.OnEvent(event);
		if (m_Synth.IsOpen())
			m_Synth.OnEvent(event);
		if (m_Player.IsOpen())
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

				ImGui::EndMenuBar();
			}

			ImGui::End();
		}

		m_SamplerController.ImGuiRender();
		m_Synth.ImGuiRender();
		m_Player.ImGuiRender();
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