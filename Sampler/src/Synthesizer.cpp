#include "Synthesizer.h"

	Synthesizer::Synthesizer()
		: m_Open(false), m_KeyboardActive(true)
	{
		m_SynthEventHandler = m_Synth.GetEventHandler();
		m_SynthParams = m_Synth.GetParams();
	}

	Synthesizer::~Synthesizer()
	{
		m_Synth.Stop();
	}

	void Synthesizer::Update()
	{
		if (!m_Open)
			m_Synth.Stop();

	}

	void Synthesizer::OnEvent(BackBeat::Event& event)
	{
		BackBeat::EventDispatcher dispatcher(event);

		dispatcher.Dispatch<BackBeat::KeyPressedEvent>(BIND_EVENT_FN(Synthesizer::OnKeyEvent));
		dispatcher.Dispatch<BackBeat::MouseButtonPressedEvent>(BIND_EVENT_FN(Synthesizer::OnMouseButtonEvent));

		if (m_Synth.IsRunning() && m_KeyboardActive) 
			event.Handled = m_SynthEventHandler->HandleEvent(event);
	}
	
	// TODO: Change sliders to match DLS 1 scaling
	void Synthesizer::ImGuiRender()
	{
		if (!m_Open)
			return;
		if (!m_Synth.IsRunning())
			m_Synth.Start();

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

		ImGui::Begin("Synth", &m_Open, synth_window_flags);

		// Render MenuBar
		{
			// TODO: Add options after features are added i.e. Menu with 'Save' to save Synth config
			if (ImGui::BeginMenuBar())
			{
				if (ImGui::BeginMenu("Devices"))
				{
					// Keyboard
					if (ImGui::BeginMenu("Keyboard"))
					{
						if (ImGui::MenuItem("Set Active", "", &m_KeyboardActive))
						{
							m_Synth.Stop();
							m_Synth.Start();
						}

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
						ImGui::EndMenu();
					}
					
					// TODO: Add MIDIDevice controls
					

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
			ImGui::SeparatorText("General Controls");
			
			static int* octave = &(m_SynthParams->eventHandlerParams->octave);
			ImGui::Text("Octave: %d", *octave); ImGui::SameLine();
			if (ImGui::SmallButton("+")) 
			{
				if (*octave < BackBeat::SynthBase::HighestOctaveSynth)
					(*octave)++;
			} ImGui::SameLine();
			if (ImGui::SmallButton("-")) 
			{
				if (*octave > BackBeat::SynthBase::LowestOctaveSynth)
					(*octave)--;
			} ImGui::SameLine(); HelpMarker("KEYBOARD ONLY");
			
			static int velocity = BackBeat::MIDI::MaxVelocity;
			ImGui::Text("    "); ImGui::SameLine(); 
			ImGui::SliderInt("Note Velocity", &velocity, BackBeat::MIDI::MinVelocity, BackBeat::MIDI::MaxVelocity);
			ImGui::SameLine(); HelpMarker("KEYBOARD ONLY \nSlider to emulate how 'hard' the note was pressed");
			m_SynthParams->eventHandlerParams->noteVelocity = (byte)velocity;
			ImGui::Spacing();
		}

		// Volume and panning controls
		{
			static float* volume = &(m_SynthParams->engineParams->volume);
			ImGui::Text("    "); ImGui::SameLine(); ImGui::SliderFloat("Volume", volume, 0.0f, 1.0f);
			ImGui::Spacing();

			static float pan = BackBeat::SynthBase::PanDefault; // Note: Actually .70 in DLS 1
			const float defaultAmp = 0.50f;
			ImGui::Text("Panning"); ImGui::SameLine();
			if (ImGui::SmallButton("Reset"))
				pan = 0.0f;
			ImGui::Text("Left"); ImGui::SameLine(); ImGui::SliderFloat("Right", &pan, BackBeat::SynthBase::PanMin, BackBeat::SynthBase::PanMax);
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
			ImGui::Text("    "); ImGui::SameLine(); ImGui::SliderFloat("LFO 1 Frequency", LFOFreq1, BackBeat::SynthBase::LFOFrequencyMin, BackBeat::SynthBase::LFOFrequencyMax);
			ImGui::Spacing();

			static float* LFOAmp1 = &(m_SynthParams->engineParams->voiceParams->LFOParams1->amp);
			ImGui::Text("    "); ImGui::SameLine(); ImGui::SliderFloat("LFO 1 Amp", LFOAmp1, BackBeat::SynthBase::LFOAttentuationMin, BackBeat::SynthBase::LFOAttentuationMax);
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
			ImGui::Text("    "); ImGui::SameLine(); ImGui::SliderFloat("Cutoff Frequency", lpCutoffFreq, BackBeat::SynthBase::FilterCutoffMin, BackBeat::SynthBase::FilterCutoffMax);
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
			ImGui::Text("    "); ImGui::SameLine(); ImGui::SliderFloat("Cutoff Frequency", hpCutoffFreq, BackBeat::SynthBase::FilterCutoffMin, BackBeat::SynthBase::FilterCutoffMax);
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
			ImGui::Text("    "); ImGui::SameLine(); ImGui::SliderFloat("Attack ", attackDuration, BackBeat::SynthBase::EG1AttackTimeMin, BackBeat::SynthBase::EG1AttackTimeMax);
			ImGui::Text("    "); ImGui::SameLine(); ImGui::SliderFloat("Decay  ", decayDuration, BackBeat::SynthBase::EG1DecayTimeMin, BackBeat::SynthBase::EG1DecayTimeMax);
			ImGui::Text("    "); ImGui::SameLine(); ImGui::SliderFloat("Release", releaseDuration, BackBeat::SynthBase::EG1ReleaseTimeMin, BackBeat::SynthBase::EG1ReleaseTimeMax);
			ImGui::Text("    "); ImGui::SameLine(); ImGui::SliderFloat("Sustain", sustain, BackBeat::SynthBase::EG1SustainLevelMin, BackBeat::SynthBase::EG1SustainLevelMax);
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
			if (ImGui::SmallButton("+")) 
			{
				if (octave1 < 2)
					octave1++;
			} ImGui::SameLine();
			if (ImGui::SmallButton("-")) 
			{
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
			if (ImGui::SmallButton("+")) 
			{
				if (octave2 < 2)
					octave2++;
			} ImGui::SameLine();
			if (ImGui::SmallButton("-")) 
			{
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
			if (ImGui::SmallButton("+")) 
			{
				if (octave3 < 2)
					octave3++;
			} ImGui::SameLine();
			if (ImGui::SmallButton("-")) 
			{
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
			if (ImGui::SmallButton("+")) 
			{
				if (octave4 < 2)
					octave4++;
			} ImGui::SameLine();
			if (ImGui::SmallButton("-"))
			{
				if (octave4 > -2)
					octave4--;
			}
			m_SynthParams->engineParams->voiceParams->OscParams4->octave = pow(2.0f, (float)octave4);

			static int selected = 3;
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

		// TODO: Add ModMatrix here
			
		// TODO: Add 2D piano and/or keyboard render here

		ImGui::End();

		ImGui::PopStyleColor(count);
	}
		
	void Synthesizer::Open()
	{
		m_Open = true;
		m_Synth.Start();
	}

	void Synthesizer::Close()
	{
		m_Open = false;
		m_Synth.Stop();
	}

	bool Synthesizer::OnKeyEvent(BackBeat::KeyPressedEvent& event)
	{
		return true;
	}

	bool Synthesizer::OnMouseButtonEvent(BackBeat::MouseButtonPressedEvent& event)
	{
		return true;
	}

	unsigned int Synthesizer::SetSynthColors()
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

	// Helper functiom from ImGui::ShowDemoWindow()
	void Synthesizer::HelpMarker(const char* desc)
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