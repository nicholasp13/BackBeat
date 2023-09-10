 #include "MasterLayer.h"


// NOTE: Demonstrates Synth/Audio Engine. BackBeat can also support playback of WAV files of certain properties. Future
// implentation of other sample rates, file types to be done later or as needed 
// ------------------------- //
// FRONTEND TODO: 
// 1. Check heap allocation / start using smart pointers
// 2. Create buffer to eliminante double presses
// 3. Make prettier
// ------------------------- //
// BACKEND TODO:
// 1. Create different Wave types: Square, Triangle, and SawTooth(Ascending/Descending) (might need to expand to more oscillator classes or Wave file classes)
// 2. Create audio filters
// 3. Create Envelope generator (Creates a more natural sounding wave like striking a key instead of a uniform wave)
// 4. Create Audio Engine that holds multiple Audio objects and mixes them into one render buffer
	MasterLayer::MasterLayer(BackBeat::Window* window)
		: Layer("MasterLayer"),
		m_Window(window),
		m_Octave(1.0f),
		m_Volume(0.5f),
		m_Worker{},
		m_Player(nullptr),
		m_Oscillator(nullptr),
		m_AudioData(nullptr), // Not used in this project, for certain types of data specifically WAV files
		m_Renderer(nullptr),
		m_Loader(nullptr) // Not used in this project, only for playing audio files
	{
	}

	MasterLayer::~MasterLayer()
	{
		m_Player->Playing = false;
		if (m_Worker.joinable())
			m_Worker.join();

		// TODO: SWITCH TO SMART POINTERS
		delete m_Player;
		delete m_Renderer;
		delete m_Oscillator;
		delete m_Loader;
		delete m_AudioData;
	}

	void MasterLayer::OnAttach()
	{
		m_Player = new BackBeat::Player();
		
		m_Oscillator = new BackBeat::Oscillator(*m_Player->GetProps());
		m_Renderer = new BackBeat::Renderer(m_Oscillator, *m_Player->GetProps());
		m_Player->SetLoader(m_Renderer);
	}

	void MasterLayer::OnDetach()
	{
	}
	
	void MasterLayer::OnUpdate(DWORD timeInterval)
	{
	}

	void MasterLayer::OnEvent(BackBeat::Event& event) 
	{
		BackBeat::EventDispatcher dispatcher(event);

		dispatcher.Dispatch<BackBeat::KeyPressedEvent>(BIND_EVENT_FN(MasterLayer::OnKeyEvent));

		if (event.IsInCategory(BackBeat::EventCategoryMouse)) {
			BB_CLIENT_TRACE("MouseEvent: {0}", event);
		}

		event.Handled = true;
	}

	void MasterLayer::OnImGuiRender() 
	{	
		unsigned int width = m_Window->GetWidth();
		unsigned int height = m_Window->GetHeight();
		ImGui::SetNextWindowSize(ImVec2((float)width, (float)height));
		ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f), 0.0f);

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
		ImGui::BulletText("Octave up   "); ImGui::SameLine(); ImGui::BulletText("TAB");
		ImGui::BulletText("Octave down "); ImGui::SameLine(); ImGui::BulletText("Left SHIFT");
		ImGui::BulletText("Octave reset"); ImGui::SameLine(); ImGui::BulletText("CAPSLOCK");
		ImGui::Spacing();

		ImGui::Text(" Volume:        ");    ImGui::SameLine(); ImGui::Text("KEY:");
		ImGui::BulletText("Volume up    "); ImGui::SameLine(); ImGui::BulletText("UP");
		ImGui::BulletText("Volume down  "); ImGui::SameLine(); ImGui::BulletText("DOWN");

		ImGui::End();
	}

	bool MasterLayer::OnKeyEvent(BackBeat::KeyPressedEvent& event)
	{
		if (event.isRepeat()) {
			event.Handled = true;
			return true;
		}

		BB_CLIENT_TRACE("KeyEvent: {0}", event);

		switch (event.GetKeyCode())
		{
		case BackBeat::Key::Space:
			{
				if (!m_Player->Playing) {
					BB_CLIENT_TRACE("Playing started");
					m_Worker = std::thread(&BackBeat::Player::Play, m_Player);
				}
				else if (m_Player->Playing) {
					BB_CLIENT_TRACE("Playing paused");
					m_Player->Pause();
					m_Worker.join();
				}
				break;
			}

			// Simple Synth piano, no polyphony/chords/multiple notes yet
			// WHITE KEYS: 'ASDFGHJK'
			case BackBeat::Key::A:
			{
				m_Oscillator->SetHertz(C_NOTE * m_Octave);
				m_Renderer->Render();
				break;
			}
			case BackBeat::Key::S:
			{
				m_Oscillator->SetHertz(D_NOTE * m_Octave);
				m_Renderer->Render();
				break;
			}
			case BackBeat::Key::D:
			{
				m_Oscillator->SetHertz(E_NOTE * m_Octave);
				m_Renderer->Render();
				break;
			}
			case BackBeat::Key::F:
			{
				m_Oscillator->SetHertz(F_NOTE * m_Octave);
				m_Renderer->Render();
				break;
			}
			case BackBeat::Key::G:
			{
				m_Oscillator->SetHertz(G_NOTE * m_Octave);
				m_Renderer->Render();
				break;
			}
			case BackBeat::Key::H:
			{
				m_Oscillator->SetHertz(A_NOTE * m_Octave);
				m_Renderer->Render();
				break;
			}
			case BackBeat::Key::J:
			{
				m_Oscillator->SetHertz(B_NOTE * m_Octave);
				m_Renderer->Render();
				break;
			}
			case BackBeat::Key::K:
			{
				m_Oscillator->SetHertz(C_NOTE * HIGHER_OCTAVE * m_Octave);
				m_Renderer->Render();
				break;
			}

			// BLACK KEYS: 'WE TYU'
			case BackBeat::Key::W:
			{
				m_Oscillator->SetHertz(CSHARP_NOTE * m_Octave);
				m_Renderer->Render();
				break;
			}
			case BackBeat::Key::E:
			{
				m_Oscillator->SetHertz(DSHARP_NOTE * m_Octave);
				m_Renderer->Render();
				break;
			}
			case BackBeat::Key::T:
			{
				m_Oscillator->SetHertz(FSHARP_NOTE * m_Octave);
				m_Renderer->Render();
				break;
			}
			case BackBeat::Key::Y:
			{
				m_Oscillator->SetHertz(GSHARP_NOTE * m_Octave);
				m_Renderer->Render();
				break;
			}
			case BackBeat::Key::U:
			{
				m_Oscillator->SetHertz(ASHARP_NOTE* m_Octave);
				m_Renderer->Render();
				break;
			}

			// OCTAVE CONTROL: TAB for higher, LeftSHIFT for lower, CAPSLOCK for reset
			// Changes octave on next key press
			case BackBeat::Key::Tab:
			{
				if (m_Octave > 4.0f)
					break;
				m_Octave = m_Octave * HIGHER_OCTAVE;
				break;
			}
			case BackBeat::Key::LeftShift:
			{
				if (m_Octave < .25f)
					break;
				m_Octave = m_Octave * LOWER_OCTAVE;
				break;
			}
			case BackBeat::Key::CapsLock:
			{
				m_Octave = 1.0f;
				break;
			}

			// VOLUME CONTROL: UpArrow increases volume/amp, DownArrow decreases volume/amp
			case BackBeat::Key::Up:
			{
				if (m_Volume >= 1.0f)
					break;
				m_Volume += 0.1f;
				m_Oscillator->SetAmp(m_Volume);
				m_Renderer->Render();
				break;
			}
			case BackBeat::Key::Down:
			{
				if (m_Volume <= 0.0f)
					break;
				m_Volume -= 0.1f;
				m_Oscillator->SetAmp(m_Volume);
				m_Renderer->Render();
				break;
			}
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