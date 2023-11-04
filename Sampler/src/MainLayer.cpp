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
		m_SynthParams = m_Synth->GetParams();
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

		RenderInfo();
		RenderControls();
		RenderModMatrix();

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

	void MainLayer::RenderInfo()
	{
		ImGui::BeginTable("Keys", 2, 0, ImVec2(1200.0f, 200.0f), 100.0f);

		ImGui::TableNextColumn();

		ImGui::SeparatorText("PRESS SPACE TO PLAY");

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

		ImGui::TableNextColumn();

		ImGui::SeparatorText("BLACK KEYS:");
		ImGui::Text(" NOTES:    ");   ImGui::SameLine(); ImGui::Text("KEY:");
		ImGui::BulletText("C Sharp"); ImGui::SameLine(); ImGui::BulletText("W");
		ImGui::BulletText("D Sharp"); ImGui::SameLine(); ImGui::BulletText("E");
		ImGui::BulletText("F Sharp"); ImGui::SameLine(); ImGui::BulletText("T");
		ImGui::BulletText("G Sharp"); ImGui::SameLine(); ImGui::BulletText("Y");
		ImGui::BulletText("A Sharp"); ImGui::SameLine(); ImGui::BulletText("U");

		ImGui::Spacing();
		ImGui::EndTable();
		ImGui::Separator();
	}

	void MainLayer::RenderControls()
	{
		ImGui::SeparatorText("Note Control");
		static int* octave = &(m_SynthParams->eventHandlerParams->octave);
		ImGui::Text("Octave: %d", *octave); ImGui::SameLine();
		if (ImGui::SmallButton("+")) {
			if (*octave < 9)
				(*octave)++;
		} ImGui::SameLine();
		if (ImGui::SmallButton("-")) {
			if (*octave > -1)
				(*octave)--;
		}

		static int velocity = 60;
		ImGui::Text("    "); ImGui::SameLine(); ImGui::SliderInt("Note Velocity", &velocity, MIDI_NOTE_C_MINUS_1, MIDI_NOTE_G_9);
		m_SynthParams->eventHandlerParams->noteVelocity = (byte)velocity;

		ImGui::Spacing();

		static float* volume = &(m_SynthParams->engineParams->volume);
		ImGui::Text("    "); ImGui::SameLine(); ImGui::SliderFloat("Volume", volume, 0.0f, 1.0f);

		ImGui::Spacing();

		static float pan = PAN_DEFAULT;
		static float defaultAmp = 0.5f;
		ImGui::Text("Panning"); ImGui::SameLine();
		if (ImGui::SmallButton("Reset"))
			pan = 0.0f;
		ImGui::Text("Left"); ImGui::SameLine(); ImGui::SliderFloat("Right", &pan, PAN_MIN, PAN_MAX);
		m_SynthParams->engineParams->voiceParams->DCAParams->leftAmp = defaultAmp - pan;
		m_SynthParams->engineParams->voiceParams->DCAParams->rightAmp = defaultAmp + pan;

		ImGui::Spacing();

		static int selected = 4;
		BackBeat::WaveType* wave = &(m_SynthParams->engineParams->voiceParams->OscParams->wave);
		ImGui::SeparatorText("Oscillator1 ");
		ImGui::Text("Waveform:");
		if (ImGui::Selectable("    Sin", selected == 1))
			selected = 1;
		if (ImGui::Selectable("    Triangle", selected == 2))
			selected = 2;
		if (ImGui::Selectable("    Square", selected == 3))
			selected = 3;
		if (ImGui::Selectable("    SawtoothUp", selected == 4))
			selected = 4;
		if (ImGui::Selectable("    SawtoothDown", selected == 5))
			selected = 5;

		switch (selected)
		{

		case 1:
		{
			*wave = BackBeat::WaveType::Sin;
			break;
		}

		case 2:
		{
			*wave = BackBeat::WaveType::Triangle;
			break;
		}

		case 3:
		{
			*wave = BackBeat::WaveType::Square;
			break;
		}

		case 4:
		{
			*wave = BackBeat::WaveType::SawtoothUp;
			break;
		}

		case 5:
		{
			*wave = BackBeat::WaveType::SawtoothDown;
			break;
		}

		}

		ImGui::Spacing();

		static float* waveAmp = &(m_SynthParams->engineParams->voiceParams->OscParams->amp);
		ImGui::Text("    "); ImGui::SameLine(); ImGui::SliderFloat("Wave Amp", waveAmp, 0.0f, 1.0f);

		ImGui::Spacing();

		static float* attackDuration = &(m_SynthParams->engineParams->voiceParams->EGParams->attackDuration);
		static float* decayDuration = &(m_SynthParams->engineParams->voiceParams->EGParams->decayDuration);
		static float* releaseDuration = &(m_SynthParams->engineParams->voiceParams->EGParams->releaseDuration);
		static float* sustain = &(m_SynthParams->engineParams->voiceParams->EGParams->sustainValue);
		ImGui::SeparatorText("Envelope Generator");
		ImGui::Text("    "); ImGui::SameLine(); ImGui::SliderFloat("Attack ", attackDuration, EG1_ATTACK_TIME_MIN, EG1_ATTACK_TIME_MAX);
		ImGui::Text("    "); ImGui::SameLine(); ImGui::SliderFloat("Decay  ", decayDuration, EG1_DECAY_TIME_MIN, EG1_DECAY_TIME_MAX);
		ImGui::Text("    "); ImGui::SameLine(); ImGui::SliderFloat("Release", releaseDuration, EG1_RELEASE_TIME_MIN, EG1_RELEASE_TIME_MAX);
		ImGui::Text("    "); ImGui::SameLine(); ImGui::SliderFloat("Sustain", sustain, EG1_SUSTAIN_LEVEL_MIN, EG1_SUSTAIN_LEVEL_MAX);
		
		ImGui::Spacing();
		ImGui::Separator();
	}

	void MainLayer::RenderModMatrix()
	{

	}