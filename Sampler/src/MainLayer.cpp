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

	// NOTE: Only implements Synth part of Audio not the Playback
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
		window_flags |= ImGuiWindowFlags_MenuBar;
		bool open;

		ImGui::ShowDemoWindow();

		ImGui::Begin("Sampler", &open, window_flags);

		RenderMenubar();
		RenderControls();

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

	void MainLayer::RenderMenubar()
	{
		// TODO: Add options after features are added
		if (ImGui::BeginMenuBar()) 
		{

			if (ImGui::BeginMenu("Controls"))
			{
				ImGui::SeparatorText("PRESS SPACE TO PLAY");

				ImGui::SeparatorText("WHITE KEYS:");
				ImGui::Text(" NOTES:");   ImGui::SameLine(); ImGui::Text("KEYS:");
				ImGui::BulletText("C  "); ImGui::SameLine(); ImGui::BulletText("A");
				ImGui::BulletText("D  "); ImGui::SameLine(); ImGui::BulletText("S");
				ImGui::BulletText("E  "); ImGui::SameLine(); ImGui::BulletText("D");
				ImGui::BulletText("F  "); ImGui::SameLine(); ImGui::BulletText("F");
				ImGui::BulletText("G  "); ImGui::SameLine(); ImGui::BulletText("G");
				ImGui::BulletText("A  "); ImGui::SameLine(); ImGui::BulletText("H");
				ImGui::BulletText("B  "); ImGui::SameLine(); ImGui::BulletText("J");
				ImGui::BulletText("C  "); ImGui::SameLine(); ImGui::BulletText("K");

				ImGui::SeparatorText("BLACK KEYS:");
				ImGui::Text(" NOTES:    ");   ImGui::SameLine(); ImGui::Text("KEYS:");
				ImGui::BulletText("C Sharp"); ImGui::SameLine(); ImGui::BulletText("W");
				ImGui::BulletText("D Sharp"); ImGui::SameLine(); ImGui::BulletText("E");
				ImGui::BulletText("F Sharp"); ImGui::SameLine(); ImGui::BulletText("T");
				ImGui::BulletText("G Sharp"); ImGui::SameLine(); ImGui::BulletText("Y");
				ImGui::BulletText("A Sharp"); ImGui::SameLine(); ImGui::BulletText("U");

				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

	}

	void MainLayer::RenderControls()
	{
		// Table flags
		static ImGuiTableFlags table_flags = 0;
		table_flags |= ImGuiTableFlags_BordersH;
		table_flags |= ImGuiTableFlags_BordersV;
		ImGui::BeginTable("Controls", 2, table_flags, ImVec2(1180.0f, 300.0f), 00.0f);

		// Note controls
		ImGui::TableNextColumn();
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
		ImGui::Text("    "); ImGui::SameLine(); ImGui::SliderInt("Note Velocity", &velocity, MIN_VELOCITY, MAX_VELOCITY);
		ImGui::SameLine(); HelpMarker("Slider to emulate how 'hard' the note was pressed");
		m_SynthParams->eventHandlerParams->noteVelocity = (byte)velocity;

		ImGui::Spacing();

		// Volume and panning controls
		static float* volume = &(m_SynthParams->engineParams->volume);
		ImGui::Text("    "); ImGui::SameLine(); ImGui::SliderFloat("Volume", volume, 0.0f, 1.0f);
		ImGui::Spacing();

		static float pan = PAN_DEFAULT;
		static float defaultAmp = 0.50f;
		ImGui::Text("Panning"); ImGui::SameLine();
		if (ImGui::SmallButton("Reset"))
			pan = 0.0f;
		ImGui::Text("Left"); ImGui::SameLine(); ImGui::SliderFloat("Right", &pan, PAN_MIN, PAN_MAX);
		m_SynthParams->engineParams->voiceParams->DCAParams->leftAmp = defaultAmp - pan;
		m_SynthParams->engineParams->voiceParams->DCAParams->rightAmp = defaultAmp + pan;

		ImGui::Spacing();

		// LFO 1 Controls
		ImGui::PushID("LFO1");
		ImGui::TableNextColumn();
		ImGui::SeparatorText("LFO1 ");
		HelpMarker("Connected directly to Oscillator1");
		static int selectedLFO = 1;
		BackBeat::WaveType* waveLFO = &(m_SynthParams->engineParams->voiceParams->LFOParams1->wave);
		ImGui::Text("Waveform:");
		if (ImGui::Selectable("    Sin", selectedLFO == 1)) {
			selectedLFO = 1;
			*waveLFO = BackBeat::WaveType::Sin;
		}
		if (ImGui::Selectable("    Triangle", selectedLFO == 2)) {
			selectedLFO = 2;
			*waveLFO = BackBeat::WaveType::Triangle;
		}
		if (ImGui::Selectable("    Square", selectedLFO == 3)) {
			selectedLFO = 3;
			*waveLFO = BackBeat::WaveType::Square;
		}
		if (ImGui::Selectable("    SawtoothUp", selectedLFO == 4)) {
			selectedLFO = 4;
			*waveLFO = BackBeat::WaveType::SawtoothUp;
		}
		if (ImGui::Selectable("    SawtoothDown", selectedLFO == 5)) {
			selectedLFO = 5;
			*waveLFO = BackBeat::WaveType::SawtoothDown;
		}

		static float* LFOFreq1 = &(m_SynthParams->engineParams->voiceParams->LFOParams1->hertz);
		ImGui::Text("    "); ImGui::SameLine(); ImGui::SliderFloat("LFO 1 Frequency", LFOFreq1, LFO_FREQ_MIN, LFO_FREQ_MAX);
		ImGui::Spacing();

		static float* LFOAmp1 = &(m_SynthParams->engineParams->voiceParams->LFOParams1->amp);
		ImGui::Text("    "); ImGui::SameLine(); ImGui::SliderFloat("LFO 1 Amp", LFOAmp1, LFO_ATT_MIN, LFO_ATT_MAX);
		ImGui::Spacing();
		ImGui::PopID();

		// Low Pass Filter Controls
		ImGui::TableNextColumn();
		ImGui::PushID("LPFilter");
		ImGui::SeparatorText("Low Pass Filter");
		static bool* lpFilterOn = &(m_SynthParams->engineParams->voiceParams->LPFilterParams->isOn);
		ImGui::Checkbox("Filter On", lpFilterOn);
		static float* lpCutoffFreq = &(m_SynthParams->engineParams->voiceParams->LPFilterParams->cutoff);
		ImGui::Text("    "); ImGui::SameLine(); ImGui::SliderFloat("Cutoff Frequency", lpCutoffFreq, FILTER_CUTOFF_MIN, FILTER_CUTOFF_MAX);
		ImGui::Spacing();
		ImGui::PopID();

		// High Pass Filter Controls
		ImGui::PushID("HPFilter");
		ImGui::SeparatorText("High Pass Filter");
		static bool* hpFilterOn = &(m_SynthParams->engineParams->voiceParams->HPFilterParams->isOn);
		ImGui::Checkbox("Filter On", hpFilterOn);
		static float* hpCutoffFreq = &(m_SynthParams->engineParams->voiceParams->HPFilterParams->cutoff);
		ImGui::Text("    "); ImGui::SameLine(); ImGui::SliderFloat("Cutoff Frequency", hpCutoffFreq, FILTER_CUTOFF_MIN, FILTER_CUTOFF_MAX);
		ImGui::Spacing();
		ImGui::PopID();

		// Amp Envelope Generator Controls
		ImGui::TableNextColumn();
		static float* attackDuration = &(m_SynthParams->engineParams->voiceParams->AmpEGParams->attackDuration);
		static float* decayDuration = &(m_SynthParams->engineParams->voiceParams->AmpEGParams->decayDuration);
		static float* releaseDuration = &(m_SynthParams->engineParams->voiceParams->AmpEGParams->releaseDuration);
		static float* sustain = &(m_SynthParams->engineParams->voiceParams->AmpEGParams->sustainValue);
		ImGui::SeparatorText("Amp Envelope Generator");
		ImGui::Text("    "); ImGui::SameLine(); ImGui::SliderFloat("Attack ", attackDuration, EG1_ATTACK_TIME_MIN, EG1_ATTACK_TIME_MAX);
		ImGui::Text("    "); ImGui::SameLine(); ImGui::SliderFloat("Decay  ", decayDuration, EG1_DECAY_TIME_MIN, EG1_DECAY_TIME_MAX);
		ImGui::Text("    "); ImGui::SameLine(); ImGui::SliderFloat("Release", releaseDuration, EG1_RELEASE_TIME_MIN, EG1_RELEASE_TIME_MAX);
		ImGui::Text("    "); ImGui::SameLine(); ImGui::SliderFloat("Sustain", sustain, EG1_SUSTAIN_LEVEL_MIN, EG1_SUSTAIN_LEVEL_MAX);

		ImGui::Spacing();

		// Osc 1 Controls
		ImGui::PushID("Osc1");
		ImGui::TableNextColumn();
		ImGui::SeparatorText("Oscillator1 ");

		static int octave1 = 0;
		ImGui::Text("Octave: %d", octave1); ImGui::SameLine();
		if (ImGui::SmallButton("+")) {
			if (octave1 < 2)
				octave1++;
		} ImGui::SameLine();
		if (ImGui::SmallButton("-")) {
			if (octave1 > -2)
				octave1--;
		}
		m_SynthParams->engineParams->voiceParams->OscParams1->octave = pow(2.0f, (float)octave1);

		static int selected1 = 4;
		BackBeat::WaveType* wave1 = &(m_SynthParams->engineParams->voiceParams->OscParams1->wave);
		ImGui::Text("Waveform:");
		if (ImGui::Selectable("    Sin", selected1 == 1)) {
			selected1 = 1;
			*wave1 = BackBeat::WaveType::Sin;
		}
		if (ImGui::Selectable("    Triangle", selected1 == 2)) {
			selected1 = 2;
			*wave1 = BackBeat::WaveType::Triangle;
		}
		if (ImGui::Selectable("    Square", selected1 == 3)) {
			selected1 = 3;
			*wave1 = BackBeat::WaveType::Square;
		}
		if (ImGui::Selectable("    SawtoothUp", selected1 == 4)) {
			selected1 = 4;
			*wave1 = BackBeat::WaveType::SawtoothUp;
		}
		if (ImGui::Selectable("    SawtoothDown", selected1 == 5)) {
			selected1 = 5;
			*wave1 = BackBeat::WaveType::SawtoothDown;
		}

		static float* waveAmp1 = &(m_SynthParams->engineParams->voiceParams->OscParams1->amp);
		ImGui::Text("    "); ImGui::SameLine(); ImGui::SliderFloat("Wave 1 Amp", waveAmp1, 0.0f, 1.0f);
		ImGui::Spacing();
		ImGui::PopID();

		// Osc 2 Controls
		ImGui::PushID("Osc2");
		ImGui::TableNextColumn();
		ImGui::SeparatorText("Oscillator2 ");

		static int octave2 = 0;
		ImGui::Text("Octave: %d", octave2); ImGui::SameLine();
		if (ImGui::SmallButton("+")) {
			if (octave2 < 2)
				octave2++;
		} ImGui::SameLine();
		if (ImGui::SmallButton("-")) {
			if (octave2 > -2)
				octave2--;
		}
		m_SynthParams->engineParams->voiceParams->OscParams2->octave = pow(2.0f, (float)octave2);

		static int selected2 = 4;
		BackBeat::WaveType* wave2 = &(m_SynthParams->engineParams->voiceParams->OscParams2->wave);
		ImGui::Text("Waveform:");
		if (ImGui::Selectable("    Sin", selected2 == 1)) {
			selected2 = 1;
			*wave2 = BackBeat::WaveType::Sin;
		}
		if (ImGui::Selectable("    Triangle", selected2 == 2)) {
			selected2 = 2;
			*wave2 = BackBeat::WaveType::Triangle;
		}
		if (ImGui::Selectable("    Square", selected2 == 3)) {
			selected2 = 3;
			*wave2 = BackBeat::WaveType::Square;
		}
		if (ImGui::Selectable("    SawtoothUp", selected2 == 4)) {
			selected2 = 4;
			*wave2 = BackBeat::WaveType::SawtoothUp;
		}
		if (ImGui::Selectable("    SawtoothDown", selected2 == 5)) {
			selected2 = 5;
			*wave2 = BackBeat::WaveType::SawtoothDown;
		}

		static float* waveAmp2 = &(m_SynthParams->engineParams->voiceParams->OscParams2->amp);
		ImGui::Text("    "); ImGui::SameLine(); ImGui::SliderFloat("Wave 2 Amp", waveAmp2, 0.0f, 1.0f);
		ImGui::Spacing();
		ImGui::PopID();

		// Osc 3 Controls
		ImGui::PushID("Osc3");
		ImGui::TableNextColumn();
		ImGui::SeparatorText("Oscillator3 ");

		static int octave3 = 0;
		ImGui::Text("Octave: %d", octave3); ImGui::SameLine();
		if (ImGui::SmallButton("+")) {
			if (octave3 < 2)
				octave3++;
		} ImGui::SameLine();
		if (ImGui::SmallButton("-")) {
			if (octave3 > -2)
				octave3--;
		}
		m_SynthParams->engineParams->voiceParams->OscParams3->octave = pow(2.0f, (float)octave3);

		static int selected3 = 4;
		BackBeat::WaveType* wave3 = &(m_SynthParams->engineParams->voiceParams->OscParams3->wave);
		ImGui::Text("Waveform:");
		if (ImGui::Selectable("    Sin", selected3 == 1)) {
			selected3 = 1;
			*wave3 = BackBeat::WaveType::Sin;
		}
		if (ImGui::Selectable("    Triangle", selected3 == 2)) {
			selected3 = 2;
			*wave3 = BackBeat::WaveType::Triangle;
		}
		if (ImGui::Selectable("    Square", selected3 == 3)) {
			selected3 = 3;
			*wave3 = BackBeat::WaveType::Square;
		}
		if (ImGui::Selectable("    SawtoothUp", selected3 == 4)) {
			selected3 = 4;
			*wave3 = BackBeat::WaveType::SawtoothUp;
		}
		if (ImGui::Selectable("    SawtoothDown", selected3 == 5)) {
			selected3 = 5;
			*wave3 = BackBeat::WaveType::SawtoothDown;
		}

		static float* waveAmp3 = &(m_SynthParams->engineParams->voiceParams->OscParams3->amp);
		ImGui::Text("    "); ImGui::SameLine(); ImGui::SliderFloat("Wave 3 Amp", waveAmp3, 0.0f, 1.0f);
		ImGui::Spacing();
		ImGui::PopID();

		// Osc 4 Controls
		ImGui::PushID("Osc4");
		ImGui::TableNextColumn();
		ImGui::SeparatorText("Oscillator4");

		static int octave4 = 0;
		ImGui::Text("Octave: %d", octave4); ImGui::SameLine();
		if (ImGui::SmallButton("+")) {
			if (octave4 < 2)
				octave4++;
		} ImGui::SameLine();
		if (ImGui::SmallButton("-")) {
			if (octave4 > -2)
				octave4--;
		}
		m_SynthParams->engineParams->voiceParams->OscParams4->octave = pow(2.0f, (float)octave4);

		static int selected4 = 4;
		BackBeat::WaveType* wave4 = &(m_SynthParams->engineParams->voiceParams->OscParams4->wave);
		ImGui::Text("Waveform:");
		if (ImGui::Selectable("    Sin", selected4 == 1)) {
			selected4 = 1;
			*wave4 = BackBeat::WaveType::Sin;
		}
		if (ImGui::Selectable("    Triangle", selected4 == 2)) {
			selected4 = 2;
			*wave4 = BackBeat::WaveType::Triangle;
		}
		if (ImGui::Selectable("    Square", selected4 == 3)) {
			selected4 = 3;
			*wave4 = BackBeat::WaveType::Square;
		}
		if (ImGui::Selectable("    SawtoothUp", selected4 == 4)) {
			selected4 = 4;
			*wave4 = BackBeat::WaveType::SawtoothUp;
		}
		if (ImGui::Selectable("    SawtoothDown", selected4 == 5)) {
			selected4 = 5;
			*wave4 = BackBeat::WaveType::SawtoothDown;
		}

		static float* waveAmp4 = &(m_SynthParams->engineParams->voiceParams->OscParams4->amp);
		ImGui::Text("    "); ImGui::SameLine(); ImGui::SliderFloat("Wave 4 Amp", waveAmp4, 0.0f, 1.0f);
		ImGui::Spacing();
		ImGui::PopID();

		ImGui::EndTable();
	}

	void MainLayer::HelpMarker(const char* desc)
	{
		ImGui::TextDisabled("(?)");
		if (ImGui::BeginItemTooltip())
		{
			ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
			ImGui::TextUnformatted(desc);
			ImGui::PopTextWrapPos();
			ImGui::EndTooltip();
		}
	}