 #include "MainLayer.h"

static bool s_OpenSynth = true;

	MainLayer::MainLayer(BackBeat::Window* window)
		: Layer("MainLayer"), m_Window(window)
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

			ImGui::Begin("Backgroumd", &open, window_flags);
			RenderBackgroundMenuBar();
			ImGui::End();
		}

		// Render Synth
		{
			if (s_OpenSynth)
			{
				if (!m_Synth->IsRunning())
					m_Synth->Start();

				const ImGuiViewport* mainViewport = ImGui::GetMainViewport();
				float x = mainViewport->WorkPos.x;
				float y = mainViewport->WorkPos.y;
				ImGui::SetNextWindowPos(ImVec2(x, y), ImGuiCond_FirstUseEver);
				ImGui::SetNextWindowSize(ImVec2(1200.0f, 600.0f), ImGuiCond_Once);
				// Synth flags
				ImGuiWindowFlags synth_window_flags = 0;
				synth_window_flags |= ImGuiWindowFlags_NoCollapse;
				synth_window_flags |= ImGuiWindowFlags_MenuBar;
				synth_window_flags |= ImGuiWindowFlags_NoResize;

				unsigned int count = SetSynthColors();

				ImGui::Begin("Synth", &s_OpenSynth, synth_window_flags);
				RenderSynth();
				ImGui::End();

				ImGui::PopStyleColor(count);
			}
			
			else if (m_Synth->IsRunning())
			{
				m_Synth->Stop();
			}
		}
	}

	bool MainLayer::OnKeyEvent(BackBeat::KeyPressedEvent& event)
	{

		if (event.isRepeat()) {
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

	void MainLayer::RenderBackgroundMenuBar()
	{
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Synth"))
			{
				if (ImGui::MenuItem("Open"))
				{
					s_OpenSynth = true;
				}
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}
	}

	// TODO: 
	//   Change slider controls as they are linear while standard Synth controls which are usually logarithmic
	//     NOTE: Check MMA DLS 1 and 2 standards as they specify controls. Also will probably need helper 
	//     functions (to defined in main BackBeat project) to scale properly with standard units like decibels.
	void MainLayer::RenderSynth()
	{
		// Render MenuBar
		{
			// TODO: Add options after features are added i.e. Menu with 'Save' to save Synth config
			if (ImGui::BeginMenuBar())
			{

				if (ImGui::BeginMenu("Controls"))
				{
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

		// Render Controls
		// Table flags
		static ImGuiTableFlags table_flags = 0;
		table_flags |= ImGuiTableFlags_RowBg;
		table_flags |= ImGuiTableFlags_BordersH;
		table_flags |= ImGuiTableFlags_BordersV;
		ImGui::BeginTable("Controls", 2, table_flags, ImVec2(0.0f, 0.0f), 0.0f);

		// Note controls
		{
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
		}

		// Volume and panning controls
		{
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
		}

		const char* waveTypes[] = { "Sin", "Triangle", "Square", "SawtoothUp", "SawtoothDown" };
		const int numWaveforms = 5;

		// LFO 1 Controls
		{
			ImGui::PushID("LFO1");
			ImGui::TableNextColumn();
			ImGui::SeparatorText("LFO1 ");
			HelpMarker("Connected directly to Oscillator1");
			static int selected = 0;
			BackBeat::WaveType* wave = &(m_SynthParams->engineParams->voiceParams->LFOParams1->wave);
			ImGui::Text("    "); ImGui::SameLine(); ImGui::Combo("Waveform", &selected, waveTypes, numWaveforms, numWaveforms);

			switch (selected)
			{

			case (0):
			{
				*wave = BackBeat::WaveType::Sin;
				break;
			}

			case (1):
			{
				*wave = BackBeat::WaveType::Triangle;
				break;
			}

			case (2):
			{
				*wave = BackBeat::WaveType::Square;
				break;
			}

			case (3):
			{
				*wave = BackBeat::WaveType::SawtoothUp;
				break;
			}

			case (4):
			{
				*wave = BackBeat::WaveType::SawtoothDown;
				break;
			}

			}
			ImGui::Spacing(); ImGui::Spacing();

			static float* LFOFreq1 = &(m_SynthParams->engineParams->voiceParams->LFOParams1->hertz);
			ImGui::Text("    "); ImGui::SameLine(); ImGui::SliderFloat("LFO 1 Frequency", LFOFreq1, LFO_FREQ_MIN, LFO_FREQ_MAX);
			ImGui::Spacing();

			static float* LFOAmp1 = &(m_SynthParams->engineParams->voiceParams->LFOParams1->amp);
			ImGui::Text("    "); ImGui::SameLine(); ImGui::SliderFloat("LFO 1 Amp", LFOAmp1, LFO_ATT_MIN, LFO_ATT_MAX);
			ImGui::Spacing();
			ImGui::PopID();
		}

		// Low Pass Filter Controls
		{
			ImGui::TableNextColumn();
			ImGui::PushID("LPFilter");
			ImGui::SeparatorText("Low Pass Filter");
			static bool* lpFilterOn = &(m_SynthParams->engineParams->voiceParams->LPFilterParams->isOn);
			ImGui::Checkbox("Filter On", lpFilterOn);
			static float* lpCutoffFreq = &(m_SynthParams->engineParams->voiceParams->LPFilterParams->cutoff);
			ImGui::Text("    "); ImGui::SameLine(); ImGui::SliderFloat("Cutoff Frequency", lpCutoffFreq, FILTER_CUTOFF_MIN, FILTER_CUTOFF_MAX);
			ImGui::Spacing();
			ImGui::PopID();
		}

		// High Pass Filter Controls
		{
			ImGui::PushID("HPFilter");
			ImGui::SeparatorText("High Pass Filter");
			static bool* hpFilterOn = &(m_SynthParams->engineParams->voiceParams->HPFilterParams->isOn);
			ImGui::Checkbox("Filter On", hpFilterOn);
			static float* hpCutoffFreq = &(m_SynthParams->engineParams->voiceParams->HPFilterParams->cutoff);
			ImGui::Text("    "); ImGui::SameLine(); ImGui::SliderFloat("Cutoff Frequency", hpCutoffFreq, FILTER_CUTOFF_MIN, FILTER_CUTOFF_MAX);
			ImGui::Spacing();
			ImGui::PopID();
		}

		// Amp Envelope Generator Controls
		{
			ImGui::PushID("AmpEG");
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
			ImGui::PopID();
		}

		// Osc 1 Controls
		{
			ImGui::PushID("Osc1");
			ImGui::TableNextColumn();
			ImGui::SeparatorText("Oscillator 1");

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

			static int selected = 3;
			BackBeat::WaveType* wave = &(m_SynthParams->engineParams->voiceParams->OscParams1->wave);
			ImGui::Text("    "); ImGui::SameLine(); ImGui::Combo("Waveform", &selected, waveTypes, numWaveforms, numWaveforms);
			
			switch (selected)
			{

			case (0):
			{
				*wave = BackBeat::WaveType::Sin;
				break;
			}

			case (1):
			{
				*wave = BackBeat::WaveType::Triangle;
				break;
			}

			case (2):
			{
				*wave = BackBeat::WaveType::Square;
				break;
			}

			case (3):
			{
				*wave = BackBeat::WaveType::SawtoothUp;
				break;
			}

			case (4):
			{
				*wave = BackBeat::WaveType::SawtoothDown;
				break;
			}

			}
			ImGui::Spacing(); ImGui::Spacing();

			static float* waveAmp1 = &(m_SynthParams->engineParams->voiceParams->OscParams1->amp);
			ImGui::Text("    "); ImGui::SameLine(); ImGui::SliderFloat("Wave 1 Amp", waveAmp1, 0.0f, 1.0f);
			ImGui::Spacing();
			ImGui::PopID();
		}

		// Osc 2 Controls
		{
			ImGui::PushID("Osc2");
			ImGui::TableNextColumn();
			ImGui::SeparatorText("Oscillator 2");

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

			static int selected = 3;
			BackBeat::WaveType* wave = &(m_SynthParams->engineParams->voiceParams->OscParams2->wave);
			ImGui::Text("    "); ImGui::SameLine(); ImGui::Combo("Waveform", &selected, waveTypes, numWaveforms, numWaveforms);

			switch (selected)
			{

			case (0):
			{
				*wave = BackBeat::WaveType::Sin;
				break;
			}

			case (1):
			{
				*wave = BackBeat::WaveType::Triangle;
				break;
			}

			case (2):
			{
				*wave = BackBeat::WaveType::Square;
				break;
			}

			case (3):
			{
				*wave = BackBeat::WaveType::SawtoothUp;
				break;
			}

			case (4):
			{
				*wave = BackBeat::WaveType::SawtoothDown;
				break;
			}

			}
			ImGui::Spacing(); ImGui::Spacing();

			static float* waveAmp2 = &(m_SynthParams->engineParams->voiceParams->OscParams2->amp);
			ImGui::Text("    "); ImGui::SameLine(); ImGui::SliderFloat("Wave 2 Amp", waveAmp2, 0.0f, 1.0f);
			ImGui::Spacing();
			ImGui::PopID();
		}

		// Osc 3 Controls
		{
			ImGui::PushID("Osc3");
			ImGui::TableNextColumn();
			ImGui::SeparatorText("Oscillator 3");

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

			static int selected = 3;
			BackBeat::WaveType* wave = &(m_SynthParams->engineParams->voiceParams->OscParams3->wave);
			ImGui::Text("    "); ImGui::SameLine(); ImGui::Combo("Waveform", &selected, waveTypes, numWaveforms, numWaveforms);

			switch (selected)
			{

			case (0):
			{
				*wave = BackBeat::WaveType::Sin;
				break;
			}

			case (1):
			{
				*wave = BackBeat::WaveType::Triangle;
				break;
			}

			case (2):
			{
				*wave = BackBeat::WaveType::Square;
				break;
			}

			case (3):
			{
				*wave = BackBeat::WaveType::SawtoothUp;
				break;
			}

			case (4):
			{
				*wave = BackBeat::WaveType::SawtoothDown;
				break;
			}

			}
			ImGui::Spacing(); ImGui::Spacing();

			static float* waveAmp3 = &(m_SynthParams->engineParams->voiceParams->OscParams3->amp);
			ImGui::Text("    "); ImGui::SameLine(); ImGui::SliderFloat("Wave 3 Amp", waveAmp3, 0.0f, 1.0f);
			ImGui::Spacing();
			ImGui::PopID();
		}

		// Osc 4 Controls
		{
			ImGui::PushID("Osc4");
			ImGui::TableNextColumn();
			ImGui::SeparatorText("Oscillator 4");

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

			static int selected = 4;
			BackBeat::WaveType* wave = &(m_SynthParams->engineParams->voiceParams->OscParams4->wave);
			ImGui::Text("    "); ImGui::SameLine(); ImGui::Combo("Waveform", &selected, waveTypes, numWaveforms, numWaveforms);

			switch (selected)
			{

			case (0):
			{
				*wave = BackBeat::WaveType::Sin;
				break;
			}

			case (1):
			{
				*wave = BackBeat::WaveType::Triangle;
				break;
			}

			case (2):
			{
				*wave = BackBeat::WaveType::Square;
				break;
			}

			case (3):
			{
				*wave = BackBeat::WaveType::SawtoothUp;
				break;
			}

			case (4):
			{
				*wave = BackBeat::WaveType::SawtoothDown;
				break;
			}

			}
			ImGui::Spacing(); ImGui::Spacing();

			static float* waveAmp4 = &(m_SynthParams->engineParams->voiceParams->OscParams4->amp);
			ImGui::Text("    "); ImGui::SameLine(); ImGui::SliderFloat("Wave 4 Amp", waveAmp4, 0.0f, 1.0f);
			ImGui::Spacing();
			ImGui::PopID();
		}

		ImGui::EndTable();
		// TODO: Add 2D piano render here
	}

	// NOTE: Call PushStyleColor
	unsigned int MainLayer::SetSynthColors()
	{
		unsigned int count = 0;

		// MenuBar colors
		ImGui::PushStyleColor(ImGuiCol_MenuBarBg, IM_COL32(91, 99, 115, 255)); count++;

		// Window colors
		ImGui::PushStyleColor(ImGuiCol_WindowBg, IM_COL32(44, 49, 56, 255)); count++;
		ImGui::PushStyleColor(ImGuiCol_Border, IM_COL32(91, 99, 115, 255)); count++;

		// Table colors
		ImGui::PushStyleColor(ImGuiCol_TableRowBg, IM_COL32(24, 29, 38, 255)); count++;
		ImGui::PushStyleColor(ImGuiCol_TableRowBgAlt, IM_COL32(24, 29, 38, 255)); count++;
		ImGui::PushStyleColor(ImGuiCol_TableBorderStrong, IM_COL32(44, 49, 56, 255)); count++;
		ImGui::PushStyleColor(ImGuiCol_TableBorderLight, IM_COL32(44, 49, 56, 255)); count++;

		// Misc colors
		ImGui::PushStyleColor(ImGuiCol_Separator, IM_COL32(91, 99, 115, 255)); count++;

		return count;
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