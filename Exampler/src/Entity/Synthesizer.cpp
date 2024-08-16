// TODO: 
//      - Fix bug where changing the general octave range causes the note to hold forever if the original octave 
//       is not returned to by the user. (NOTE: MIDI Devices handle this by not changing the MIDINote of keys that are
//       currently pressed when the MIDI user changes the octave range on his MIDI device)
//      - Fix bug where the parameters aren't set before open so the volume changes when the user first opens this

#include "Synthesizer.h"
namespace Exampler {

	Synthesizer::Synthesizer()
		: 
		m_Open(false), 
		m_KeyboardActive(true),
		m_NoteVelocity(BackBeat::MIDI::MaxVelocity),
		m_Pan(BackBeat::SynthBase::PanDefault),
		m_LFOWave(0),
		m_OscWave1(3),
		m_OscWave2(3),
		m_OscWave3(3),
		m_OscWave4(3),
		m_Octave1(0),
		m_Octave2(0),
		m_Octave3(0),
		m_Octave4(0),
		m_RecordingPlayer(nullptr),
		m_RecorderMgr(nullptr)
	{
		m_SynthEventHandler = m_Synth.GetEventHandler();
		m_SynthParams = m_Synth.GetParams();
	}

	Synthesizer::~Synthesizer()
	{
		m_Synth.Stop();
		m_RecordingPlayer->Stop();
	}

	void Synthesizer::Update()
	{

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
		auto synthID = m_Synth.GetID();
		ImGui::PushID(synthID.ToString().c_str());
		unsigned int count = SetSynthColors();

		RenderCanvasEntity();

		if (!m_Open)
		{
			ImGui::PopStyleColor(count);
			ImGui::PopID();
			return;
		}

		// Renders Actual Entity
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

		// Creates a label ID for ImGui::Begin() that avoids collision to other ImGui::Begin() calls with the same name
		const std::string hashDivider = "###";
		std::string labelID = m_Name.c_str() + hashDivider + synthID.ToString();
		ImGui::Begin(labelID.c_str(), &m_Open, synth_window_flags);
		
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

					ImGui::EndMenu();
				}
				ImGui::EndMenuBar();
			}
		}

		// Render Controls
		// Table flags
		ImGuiTableFlags table_flags = 0;
		table_flags |= ImGuiTableFlags_RowBg;
		table_flags |= ImGuiTableFlags_BordersH;
		table_flags |= ImGuiTableFlags_BordersV;
		ImGui::BeginTable("Controls", 2, table_flags, ImVec2(0.0f, 0.0f), 0.0f);

		// Note controls
		{
			ImGui::TableNextColumn();
			ImGui::SeparatorText("General Controls");

			int* octave = &(m_SynthParams->eventHandlerParams->octave);
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

			ImGui::Text("    "); ImGui::SameLine();
			ImGui::SliderInt("Note Velocity", &m_NoteVelocity, BackBeat::MIDI::MinVelocity, BackBeat::MIDI::MaxVelocity);
			ImGui::SameLine(); HelpMarker("KEYBOARD ONLY \nSlider to emulate how 'hard' the note was pressed");
			m_SynthParams->eventHandlerParams->noteVelocity = (byte)m_NoteVelocity;
			ImGui::Spacing();
		}

		// Volume and panning controls
		{
			float* volume = &(m_SynthParams->engineParams->volume);
			ImGui::Text("    "); ImGui::SameLine(); ImGui::SliderFloat("Volume", volume, 0.0f, 1.0f);
			ImGui::Spacing();

			const float defaultAmp = 0.50f; //  Note: Actually .70 in DLS 1
			ImGui::Text("Panning"); ImGui::SameLine();
			if (ImGui::SmallButton("Reset"))
				m_Pan = 0.0f;
			ImGui::Text("Left"); ImGui::SameLine(); ImGui::SliderFloat("Right", &m_Pan, BackBeat::SynthBase::PanMin, BackBeat::SynthBase::PanMax);
			m_SynthParams->engineParams->voiceParams->DCAParams->leftAmp = defaultAmp - m_Pan;
			m_SynthParams->engineParams->voiceParams->DCAParams->rightAmp = defaultAmp + m_Pan;

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
			BackBeat::WaveType* wave = &(m_SynthParams->engineParams->voiceParams->LFOParams1->wave);
			ImGui::Text("    "); ImGui::SameLine(); ImGui::Combo("Waveform", &m_LFOWave, waveTypes, numWaveforms, numWaveforms);

			switch (m_LFOWave)
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

			float* LFOFreq1 = &(m_SynthParams->engineParams->voiceParams->LFOParams1->hertz);
			ImGui::Text("    "); ImGui::SameLine(); ImGui::SliderFloat("LFO 1 Frequency", LFOFreq1, BackBeat::SynthBase::LFOFrequencyMin, BackBeat::SynthBase::LFOFrequencyMax);
			ImGui::Spacing();

			float* LFOAmp1 = &(m_SynthParams->engineParams->voiceParams->LFOParams1->amp);
			ImGui::Text("    "); ImGui::SameLine(); ImGui::SliderFloat("LFO 1 Amp", LFOAmp1, BackBeat::SynthBase::LFOAttentuationMin, BackBeat::SynthBase::LFOAttentuationMax);
			ImGui::Spacing();
			ImGui::PopID();
		}

		// Low Pass Filter Controls
		{
			ImGui::TableNextColumn();
			ImGui::PushID("LPFilter");
			ImGui::SeparatorText("Low Pass Filter");
			bool* lpFilterOn = &(m_SynthParams->engineParams->voiceParams->LPFilterParams->isOn);
			ImGui::Checkbox("Filter On", lpFilterOn);
			float* lpCutoffFreq = &(m_SynthParams->engineParams->voiceParams->LPFilterParams->cutoff);
			ImGui::Text("    "); ImGui::SameLine(); ImGui::SliderFloat("Cutoff Frequency", lpCutoffFreq, BackBeat::SynthBase::FilterCutoffMin, BackBeat::SynthBase::FilterCutoffMax);
			ImGui::Spacing();
			ImGui::PopID();
		}

		// High Pass Filter Controls
		{
			ImGui::PushID("HPFilter");
			ImGui::SeparatorText("High Pass Filter");
			bool* hpFilterOn = &(m_SynthParams->engineParams->voiceParams->HPFilterParams->isOn);
			ImGui::Checkbox("Filter On", hpFilterOn);
			float* hpCutoffFreq = &(m_SynthParams->engineParams->voiceParams->HPFilterParams->cutoff);
			ImGui::Text("    "); ImGui::SameLine(); ImGui::SliderFloat("Cutoff Frequency", hpCutoffFreq, BackBeat::SynthBase::FilterCutoffMin, BackBeat::SynthBase::FilterCutoffMax);
			ImGui::Spacing();
			ImGui::PopID();
		}

		// Amp Envelope Generator Controls
		{
			ImGui::PushID("AmpEG");
			ImGui::TableNextColumn();
			float* attackDuration = &(m_SynthParams->engineParams->voiceParams->AmpEGParams->attackDuration);
			float* decayDuration = &(m_SynthParams->engineParams->voiceParams->AmpEGParams->decayDuration);
			float* releaseDuration = &(m_SynthParams->engineParams->voiceParams->AmpEGParams->releaseDuration);
			float* sustain = &(m_SynthParams->engineParams->voiceParams->AmpEGParams->sustainValue);
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

			ImGui::Text("Octave: %d", m_Octave1); ImGui::SameLine();
			if (ImGui::SmallButton("+"))
			{
				if (m_Octave1 < 2)
					m_Octave1++;
			} ImGui::SameLine();
			if (ImGui::SmallButton("-"))
			{
				if (m_Octave1 > -2)
					m_Octave1--;
			}
			m_SynthParams->engineParams->voiceParams->OscParams1->octave = pow(2.0f, (float)m_Octave1);

			BackBeat::WaveType* wave = &(m_SynthParams->engineParams->voiceParams->OscParams1->wave);
			ImGui::Text("    "); ImGui::SameLine(); ImGui::Combo("Waveform", &m_OscWave1, waveTypes, numWaveforms, numWaveforms);

			switch (m_OscWave1)
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

			float* waveAmp1 = &(m_SynthParams->engineParams->voiceParams->OscParams1->amp);
			ImGui::Text("    "); ImGui::SameLine(); ImGui::SliderFloat("Wave 1 Amp", waveAmp1, 0.0f, 1.0f);
			ImGui::Spacing();
			ImGui::PopID();
		}

		// Osc 2 Controls
		{
			ImGui::PushID("Osc2");
			ImGui::TableNextColumn();
			ImGui::SeparatorText("Oscillator 2");

			ImGui::Text("Octave: %d", m_Octave2); ImGui::SameLine();
			if (ImGui::SmallButton("+"))
			{
				if (m_Octave2 < 2)
					m_Octave2++;
			} ImGui::SameLine();
			if (ImGui::SmallButton("-"))
			{
				if (m_Octave2 > -2)
					m_Octave2--;
			}
			m_SynthParams->engineParams->voiceParams->OscParams2->octave = pow(2.0f, (float)m_Octave2);

			BackBeat::WaveType* wave = &(m_SynthParams->engineParams->voiceParams->OscParams2->wave);
			ImGui::Text("    "); ImGui::SameLine(); ImGui::Combo("Waveform", &m_OscWave2, waveTypes, numWaveforms, numWaveforms);

			switch (m_OscWave2)
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

			float* waveAmp2 = &(m_SynthParams->engineParams->voiceParams->OscParams2->amp);
			ImGui::Text("    "); ImGui::SameLine(); ImGui::SliderFloat("Wave 2 Amp", waveAmp2, 0.0f, 1.0f);
			ImGui::Spacing();
			ImGui::PopID();
		}

		// Osc 3 Controls
		{
			ImGui::PushID("Osc3");
			ImGui::TableNextColumn();
			ImGui::SeparatorText("Oscillator 3");

			ImGui::Text("Octave: %d", m_Octave3); ImGui::SameLine();
			if (ImGui::SmallButton("+"))
			{
				if (m_Octave3 < 2)
					m_Octave3++;
			} ImGui::SameLine();
			if (ImGui::SmallButton("-"))
			{
				if (m_Octave3 > -2)
					m_Octave3--;
			}
			m_SynthParams->engineParams->voiceParams->OscParams3->octave = pow(2.0f, (float)m_Octave3);

			BackBeat::WaveType* wave = &(m_SynthParams->engineParams->voiceParams->OscParams3->wave);
			ImGui::Text("    "); ImGui::SameLine(); ImGui::Combo("Waveform", &m_OscWave3, waveTypes, numWaveforms, numWaveforms);

			switch (m_OscWave3)
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

			float* waveAmp3 = &(m_SynthParams->engineParams->voiceParams->OscParams3->amp);
			ImGui::Text("    "); ImGui::SameLine(); ImGui::SliderFloat("Wave 3 Amp", waveAmp3, 0.0f, 1.0f);
			ImGui::Spacing();
			ImGui::PopID();
		}

		// Osc 4 Controls
		{
			ImGui::PushID("Osc4");
			ImGui::TableNextColumn();
			ImGui::SeparatorText("Oscillator 4");

			ImGui::Text("Octave: %d", m_Octave4); ImGui::SameLine();
			if (ImGui::SmallButton("+"))
			{
				if (m_Octave4 < 2)
					m_Octave4++;
			} ImGui::SameLine();
			if (ImGui::SmallButton("-"))
			{
				if (m_Octave4 > -2)
					m_Octave4--;
			}
			m_SynthParams->engineParams->voiceParams->OscParams4->octave = pow(2.0f, (float)m_Octave4);

			BackBeat::WaveType* wave = &(m_SynthParams->engineParams->voiceParams->OscParams4->wave);
			ImGui::Text("    "); ImGui::SameLine(); ImGui::Combo("Waveform", &m_OscWave4, waveTypes, numWaveforms, numWaveforms);

			switch (m_OscWave4)
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

			float* waveAmp4 = &(m_SynthParams->engineParams->voiceParams->OscParams4->amp);
			ImGui::Text("    "); ImGui::SameLine(); ImGui::SliderFloat("Wave 4 Amp", waveAmp4, 0.0f, 1.0f);
			ImGui::Spacing();
			ImGui::PopID();
		}

		ImGui::EndTable();

		// TODO: Add ModMatrix here

		ImGui::End();

		ImGui::PopStyleColor(count);
		ImGui::PopID();
	}

	void Synthesizer::Add(
		BackBeat::PlayerManager* playerMgr,
		BackBeat::RecorderManager* recorderMgr,
		BackBeat::Mixer* mixer,
		BackBeat::MIDIDeviceManager* midiDeviceManager)
	{
		m_RecorderMgr = recorderMgr;
		m_RecordingPlayer = playerMgr->AddNewPlayer();
		auto synthProc = m_Synth.GetProcessor();
		auto synthID = synthProc->GetID();

		auto synthTrack = m_RecorderMgr->AddRecordingTrack(synthID, BackBeat::RecorderType::audio);
		m_RecordingPlayer->LoadTrack(synthTrack);
		
		mixer->PushProcessor(synthProc);
		mixer->PushProcessor(m_RecordingPlayer->GetProc());
		midiDeviceManager->PushOutput(m_Synth.GetMIDIInput());
	}

	void Synthesizer::Delete(
		BackBeat::PlayerManager* playerMgr,
		BackBeat::RecorderManager* recorderMgr,
		BackBeat::Mixer* mixer,
		BackBeat::MIDIDeviceManager* midiDeviceManager)
	{
		auto synthID = m_Synth.GetID();
		auto trackPlayerID = m_RecordingPlayer->GetID();
		auto midiInputID = m_Synth.GetMIDIInput()->GetID();

		m_Synth.Stop();
		m_RecordingPlayer->Stop();

		playerMgr->Delete(m_RecordingPlayer->GetID());
		recorderMgr->DeleteTrack(synthID);
		mixer->DeleteProcessor(synthID);
		mixer->DeleteProcessor(trackPlayerID);
		midiDeviceManager->DeleteOutput(midiInputID);
	}

	void Synthesizer::RenderCanvasEntity()
	{
		auto synthID = m_Synth.GetID();

		ImGui::PushID(m_Name.c_str());
		ImGui::Spacing();
		ImGui::SeparatorText(m_Name.c_str());

		if (!m_Synth.IsRunning())
		{
			if (ImGui::Button("On "))
				On();
		}
		else
		{
			if (ImGui::Button("Off"))
			{
				m_RecorderMgr->SetRecorderInactive(synthID);
				Off();
			}
		}
		ImGui::SameLine();

		if (ImGui::Button("Open"))
			Open();

		// Render Recorder controls
		{
			if (!m_RecorderMgr->IsActive(synthID))
			{
				if (ImGui::Button("Record On", ImVec2(125, 20)))
				{
					if (!m_RecorderMgr->IsRecording())
					{
						On();
						m_RecorderMgr->SetRecorderActive(synthID);
					}
				}
			}
			else
			{
				if (ImGui::Button("Record Off", ImVec2(125, 20)))
					if (!m_RecorderMgr->IsRecording())
						m_RecorderMgr->SetRecorderInactive(synthID);
			}
			ImGui::SameLine();

		}

		// Render Recording Track Player controls
		{
			int trackSize = m_RecordingPlayer->GetSize();
			if (m_RecordingPlayer && trackSize > 0)
			{
				if (!m_RecordingPlayer->IsOn())
				{
					if (ImGui::Button("Play Recording On "))
						m_RecordingPlayer->On();
				}
				else
				{
					if (ImGui::Button("Play Recording Off"))
						m_RecordingPlayer->Off();
				} ImGui::SameLine();

				if (ImGui::Button("Clear Recording"))
					if (!m_RecorderMgr->IsActive(synthID))
						m_RecorderMgr->ResetRecording(synthID);

				int position = m_RecordingPlayer->GetPosition();

				static bool wasPlaying = false;
				BackBeat::TimeMinSec trackTime = m_RecordingPlayer->GetTime();
				BackBeat::TimeMinSec trackLength = m_RecordingPlayer->GetLength();

				ImGui::Text("%d:%02d", trackTime.minutes, trackTime.seconds); ImGui::SameLine();

				// Placeholder for future implementation of a custom ImGui::Timeline widget
				ImGui::PushID("Seekbar");
				if (BackBeat::ImGuiWidgets::ImGuiSeekBarInt("##", &position, trackSize, "", ImGuiSliderFlags(0)))
				{
					if (m_RecordingPlayer->IsPlaying())
					{
						m_RecordingPlayer->Pause();
						wasPlaying = true;
					}
					m_RecordingPlayer->SetPosition(position);
				}
				if (ImGui::IsItemDeactivated() && wasPlaying)
				{
					m_RecordingPlayer->Play();
					wasPlaying = false;
				}
				ImGui::SameLine(); ImGui::Text("%d:%02d", trackLength.minutes, trackLength.seconds);
				ImGui::PopID();

			}
			else
			{
				if (ImGui::Button("Play Recording On "))
				{ 
				} ImGui::SameLine();
				if (ImGui::Button("Clear Recording"))
				{ 
				}

				// Renders an empty, uninteractable seek bar if no track is loaded
				ImGui::PushID("EmptySeekbar");
				int temp = 0;
				ImGui::Text("%d:%02d", 0, 0); ImGui::SameLine();
				BackBeat::ImGuiWidgets::ImGuiSeekBarInt("##", &temp, 10000, "", ImGuiSliderFlags(0)); ImGui::SameLine();
				ImGui::Text("%d:%02d", 0, 0);
				ImGui::PopID();

			}
			ImGui::Spacing();

		}

		float* volume = &(m_SynthParams->engineParams->volume);
		ImGui::Text("    "); ImGui::SameLine();
		BackBeat::ImGuiWidgets::ImGuiSeekBarFloat("Volume", volume, 1.0f, "", ImGuiSliderFlags(0));
		
		ImGui::Spacing();
		ImGui::PopID();
	}

	unsigned int Synthesizer::SetSynthColors()
	{
		unsigned int count = 0;

		// MenuBar colors
		ImGui::PushStyleColor(ImGuiCol_MenuBarBg, IM_COL32(91, 99, 115, 255)); count++;

		// Window colors
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
}