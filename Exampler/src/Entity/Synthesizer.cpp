// TODO: 
//      - Fix bug where changing the general octave range causes the note to hold forever if the original octave 
//       is not returned to by the user. (NOTE: MIDI Devices handle this by not changing the MIDINote of keys that are
//       currently pressed when the MIDI user changes the octave range on his MIDI device)
//      - Fix bug where the parameters aren't set before open so the volume changes when the user first opens this

#include "Synthesizer.h"
namespace Exampler {

	// TODO: Get rid of magic numbers for 
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

	// NOTE: - node is the parent of the node being written to
	//       - TODO: Still need to implement serializing RecordingTracks
	void Synthesizer::WriteObject(pugi::xml_node* node)
	{
		auto synthNode = node->append_child("Synthesizer");

		synthNode.append_attribute("Name") = m_Name;

		// General Controls
		{
			auto keyboardControls = synthNode.append_child("KeyboardControls");
			keyboardControls.append_attribute("Active") = m_KeyboardActive;
			keyboardControls.append_attribute("Octave") = m_SynthParams->eventHandlerParams->octave;
			keyboardControls.append_attribute("NoteVelocity") = m_NoteVelocity;

			synthNode.append_child("Volume").append_attribute("Value") = m_SynthParams->engineParams->volume;

			synthNode.append_child("Pan").append_attribute("Value") = m_Pan;
		}

		// LFO 1
		{
			std::shared_ptr<BackBeat::LFOParameters> lfoParams = 
				m_SynthParams->engineParams->voiceParams->LFOParams1;

			auto lfoNode = synthNode.append_child("LFO1");
			auto waveformNode = lfoNode.append_child("Waveform");

			switch (lfoParams->wave)
			{

			case BackBeat::WaveType::Sin:
			{
				waveformNode.append_attribute("Type") = "Sin";
				break;
			}

			case BackBeat::WaveType::SawtoothUp:
			{
				waveformNode.append_attribute("Type") = "SawtoothUp";
				break;
			}

			case BackBeat::WaveType::SawtoothDown:
			{
				waveformNode.append_attribute("Type") = "SawtoothDown";
				break;
			}

			case BackBeat::WaveType::Triangle:
			{
				waveformNode.append_attribute("Type") = "Triangle";
				break;
			}

			case BackBeat::WaveType::Square:
			{
				waveformNode.append_attribute("Type") = "Square";
				break;
			}

			default:
			{
				BB_CLIENT_ERROR("UNKNOWN WAVEFORM TYPE! Saved as sin wave. Please update WaveTypes or Synthesizer");
				waveformNode.append_attribute("Type") = "Sin";
				break;
			}

			}

			lfoNode.append_child("Frequency").append_attribute("Value") = lfoParams->hertz;
			lfoNode.append_child("Amp").append_attribute("Value") = lfoParams->amp;
		}

		// Low Pass Filter
		{
			std::shared_ptr<BackBeat::FilterParameters> lowpassParams = 
				m_SynthParams->engineParams->voiceParams->LPFilterParams;

			auto lowpassNode = synthNode.append_child("LowPassFilter");

			lowpassNode.append_child("FilterOn").append_attribute("Value") = lowpassParams->isOn;
			lowpassNode.append_child("CutoffFrequency").append_attribute("Value") = lowpassParams->cutoff;
		}

		// High Pass Filter
		{
			std::shared_ptr<BackBeat::FilterParameters> highpassParams =
				m_SynthParams->engineParams->voiceParams->HPFilterParams;

			auto highpassNode = synthNode.append_child("HighPassFilter");

			highpassNode.append_child("FilterOn").append_attribute("Value") = highpassParams->isOn;
			highpassNode.append_child("CutoffFrequency").append_attribute("Value") = highpassParams->cutoff;
		}

		// Amp Envelope Generator
		{
			std::shared_ptr<BackBeat::EGParameters> ampEGParams =
				m_SynthParams->engineParams->voiceParams->AmpEGParams;

			auto ampEGNode = synthNode.append_child("AmpEnvelopeGenerator");

			ampEGNode.append_child("Attack").append_attribute("Value") = ampEGParams->attackDuration;
			ampEGNode.append_child("Decay").append_attribute("Value") = ampEGParams->decayDuration;
			ampEGNode.append_child("Release").append_attribute("Value") = ampEGParams->releaseDuration;
			ampEGNode.append_child("Sustain").append_attribute("Value") = ampEGParams->sustainValue;
		}

		// Oscillator 1
		{
			std::shared_ptr<BackBeat::OscParameters> oscParams =
				m_SynthParams->engineParams->voiceParams->OscParams1;

			auto oscNode = synthNode.append_child("Oscillator1");

			oscNode.append_child("Octave").append_attribute("Value") = m_Octave1;

			auto waveformNode = oscNode.append_child("Waveform");

			switch (oscParams->wave)
			{
			case BackBeat::WaveType::Sin:
			{
				waveformNode.append_attribute("Type") = "Sin";
				break;
			}

			case BackBeat::WaveType::SawtoothUp:
			{
				waveformNode.append_attribute("Type") = "SawtoothUp";
				break;
			}

			case BackBeat::WaveType::SawtoothDown:
			{
				waveformNode.append_attribute("Type") = "SawtoothDown";
				break;
			}

			case BackBeat::WaveType::Triangle:
			{
				waveformNode.append_attribute("Type") = "Triangle";
				break;
			}

			case BackBeat::WaveType::Square:
			{
				waveformNode.append_attribute("Type") = "Square";
				break;
			}

			default:
			{
				BB_CLIENT_ERROR("UNKNOWN WAVEFORM TYPE! Saved as sin wave. Please update WaveTypes or Synthesizer");
				waveformNode.append_attribute("Type") = "Sin";
				break;
			}

			}

			oscNode.append_child("Amp").append_attribute("Value") = oscParams->amp;
		}

		// Oscillator 2
		{
			std::shared_ptr<BackBeat::OscParameters> oscParams =
				m_SynthParams->engineParams->voiceParams->OscParams2;

			auto oscNode = synthNode.append_child("Oscillator2");

			oscNode.append_child("Octave").append_attribute("Value") = m_Octave2;

			auto waveformNode = oscNode.append_child("Waveform");

			switch (oscParams->wave)
			{
			case BackBeat::WaveType::Sin:
			{
				waveformNode.append_attribute("Type") = "Sin";
				break;
			}

			case BackBeat::WaveType::SawtoothUp:
			{
				waveformNode.append_attribute("Type") = "SawtoothUp";
				break;
			}

			case BackBeat::WaveType::SawtoothDown:
			{
				waveformNode.append_attribute("Type") = "SawtoothDown";
				break;
			}

			case BackBeat::WaveType::Triangle:
			{
				waveformNode.append_attribute("Type") = "Triangle";
				break;
			}

			case BackBeat::WaveType::Square:
			{
				waveformNode.append_attribute("Type") = "Square";
				break;
			}

			default:
			{
				BB_CLIENT_ERROR("UNKNOWN WAVEFORM TYPE! Saved as sin wave. Please update WaveTypes or Synthesizer");
				waveformNode.append_attribute("Type") = "Sin";
				break;
			}

			}

			oscNode.append_child("Amp").append_attribute("Value") = oscParams->amp;
		}

		// Oscillator 3
		{
			std::shared_ptr<BackBeat::OscParameters> oscParams =
				m_SynthParams->engineParams->voiceParams->OscParams3;

			auto oscNode = synthNode.append_child("Oscillator3");

			oscNode.append_child("Octave").append_attribute("Value") = m_Octave3;

			auto waveformNode = oscNode.append_child("Waveform");

			switch (oscParams->wave)
			{
			case BackBeat::WaveType::Sin:
			{
				waveformNode.append_attribute("Type") = "Sin";
				break;
			}

			case BackBeat::WaveType::SawtoothUp:
			{
				waveformNode.append_attribute("Type") = "SawtoothUp";
				break;
			}

			case BackBeat::WaveType::SawtoothDown:
			{
				waveformNode.append_attribute("Type") = "SawtoothDown";
				break;
			}

			case BackBeat::WaveType::Triangle:
			{
				waveformNode.append_attribute("Type") = "Triangle";
				break;
			}

			case BackBeat::WaveType::Square:
			{
				waveformNode.append_attribute("Type") = "Square";
				break;
			}

			default:
			{
				BB_CLIENT_ERROR("UNKNOWN WAVEFORM TYPE! Saved as sin wave. Please update WaveTypes or Synthesizer");
				waveformNode.append_attribute("Type") = "Sin";
				break;
			}

			}

			oscNode.append_child("Amp").append_attribute("Value") = oscParams->amp;
		}

		// Oscillator 4
		{
			std::shared_ptr<BackBeat::OscParameters> oscParams =
				m_SynthParams->engineParams->voiceParams->OscParams4;

			auto oscNode = synthNode.append_child("Oscillator4");

			oscNode.append_child("Octave").append_attribute("Value") = m_Octave4;

			auto waveformNode = oscNode.append_child("Waveform");

			switch (oscParams->wave)
			{
			case BackBeat::WaveType::Sin:
			{
				waveformNode.append_attribute("Type") = "Sin";
				break;
			}

			case BackBeat::WaveType::SawtoothUp:
			{
				waveformNode.append_attribute("Type") = "SawtoothUp";
				break;
			}

			case BackBeat::WaveType::SawtoothDown:
			{
				waveformNode.append_attribute("Type") = "SawtoothDown";
				break;
			}

			case BackBeat::WaveType::Triangle:
			{
				waveformNode.append_attribute("Type") = "Triangle";
				break;
			}

			case BackBeat::WaveType::Square:
			{
				waveformNode.append_attribute("Type") = "Square";
				break;
			}

			default:
			{
				BB_CLIENT_ERROR("UNKNOWN WAVEFORM TYPE! Saved as sin wave. Please update WaveTypes or Synthesizer");
				waveformNode.append_attribute("Type") = "Sin";
				break;
			}

			}

			oscNode.append_child("Amp").append_attribute("Value") = oscParams->amp;
		}

		// Audio track
		{
			// Copy/move track from temp folder to the project folder
			auto trackNode = synthNode.append_child("Track");

			std::string trackName = m_RecordingPlayer->GetTrackName();
			std::string fromFilePath = m_RecordingPlayer->GetTrackFilePath();
			std::string toFilePath = BackBeat::Project::GetActive()->GetConfig().tracksDirectoryPath + trackName;

			BB_CLIENT_INFO("SYNTH TRACK FORMER PATH: {0}", fromFilePath.c_str());
			BB_CLIENT_INFO("SYNTH TRACK TO PATH: {0}", toFilePath.c_str());

			// Move file here
		}
	}

	// NOTE: - node is the node being read from. This is different to WriteObject() || Might want to specify in
	//       function declaration
	//       - TODO: Still need to implement serializing RecordingTracks
	void Synthesizer::ReadObject(pugi::xml_node* node)
	{
		m_Name = node->attribute("Name").as_string();

		// General Controls
		{
			auto keyboardControls = node->child("KeyboardControls");
			m_KeyboardActive = keyboardControls.attribute("Active").as_bool();
			m_SynthParams->eventHandlerParams->octave = keyboardControls.attribute("Octave").as_int();
			m_NoteVelocity = keyboardControls.attribute("NoteVelocity").as_int();

			m_SynthParams->engineParams->volume = node->child("Volume").attribute("Value").as_float();

			m_Pan = node->child("Pan").attribute("Value").as_float();
		}

		// LFO 1
		{
			std::shared_ptr<BackBeat::LFOParameters> lfoParams =
				m_SynthParams->engineParams->voiceParams->LFOParams1;

			auto lfoNode = node->child("LFO1");
			auto waveformNode = lfoNode.child("Waveform");

			auto waveType = waveformNode.attribute("Type").value();

			if (strcmp(waveType, "Sin") == 0)
			{
				m_LFOWave = 0;
				lfoParams->wave = BackBeat::WaveType::Sin;
			}
			else if (strcmp(waveType, "SawtoothUp") == 0)
			{
				m_LFOWave = 3;
				lfoParams->wave = BackBeat::WaveType::SawtoothUp;
			}
			else if (strcmp(waveType, "SawtoothDown") == 0)
			{
				m_LFOWave = 4;
				lfoParams->wave = BackBeat::WaveType::SawtoothDown;
			}
			else if (strcmp(waveType, "Triangle") == 0)
			{
				m_LFOWave = 1;
				lfoParams->wave = BackBeat::WaveType::Triangle;
			}
			else if (strcmp(waveType, "Square") == 0)
			{
				m_LFOWave = 2;
				lfoParams->wave = BackBeat::WaveType::Square;
			}
			else
			{
				BB_CLIENT_ERROR("UNRECOGNIZED WAVETYPE FOR LFO IN XML FOR {0} TRACK! Loading Sin Wav",
					m_Name.c_str());
				lfoParams->wave = BackBeat::WaveType::Sin;
			}

			lfoParams->hertz = lfoNode.child("Frequency").attribute("Value").as_float();
			lfoParams->amp = lfoNode.child("Amp").attribute("Value").as_float();
		}

		// Low Pass Filter
		{
			std::shared_ptr<BackBeat::FilterParameters> lowpassParams =
				m_SynthParams->engineParams->voiceParams->LPFilterParams;

			auto lowpassNode = node->child("LowPassFilter");

			lowpassParams->isOn = lowpassNode.child("FilterOn").attribute("Value").as_bool();
			lowpassParams->cutoff = lowpassNode.child("CutoffFrequency").attribute("Value").as_float();
		}

		// High Pass Filter
		{
			std::shared_ptr<BackBeat::FilterParameters> highpassParams =
				m_SynthParams->engineParams->voiceParams->HPFilterParams;

			auto lowpassNode = node->child("HighPassFilter");

			highpassParams->isOn = lowpassNode.child("FilterOn").attribute("Value").as_bool();
			highpassParams->cutoff = lowpassNode.child("CutoffFrequency").attribute("Value").as_float();
		}

		// Amp Envelope Generator
		{
			std::shared_ptr<BackBeat::EGParameters> ampEGParams =
				m_SynthParams->engineParams->voiceParams->AmpEGParams;

			auto ampEGNode = node->child("AmpEnvelopeGenerator");

			ampEGParams->attackDuration = ampEGNode.child("Attack").attribute("Value").as_float();
			ampEGParams->decayDuration = ampEGNode.child("Decay").attribute("Value").as_float();
			ampEGParams->releaseDuration = ampEGNode.child("Release").attribute("Value").as_float();
			ampEGParams->sustainValue = ampEGNode.child("Sustain").attribute("Value").as_float();
		}

		// Oscillator 1
		{
			std::shared_ptr<BackBeat::OscParameters> oscParams =
				m_SynthParams->engineParams->voiceParams->OscParams1;

			auto oscNode = node->child("Oscillator1");

			m_Octave1 = oscNode.child("Octave").attribute("Value").as_int();
			oscParams->octave = pow(2.0f, (float)m_Octave1);

			auto waveformNode = oscNode.child("Waveform");

			auto waveType = waveformNode.attribute("Type").value();

			if (strcmp(waveType, "Sin") == 0)
			{
				m_OscWave1 = 0;
				oscParams->wave = BackBeat::WaveType::Sin;
			}
			else if (strcmp(waveType, "SawtoothUp") == 0)
			{
				m_OscWave1 = 3;
				oscParams->wave = BackBeat::WaveType::SawtoothUp;
			}
			else if (strcmp(waveType, "SawtoothDown") == 0)
			{
				m_OscWave1 = 4;
				oscParams->wave = BackBeat::WaveType::SawtoothDown;
			}
			else if (strcmp(waveType, "Triangle") == 0)
			{
				m_OscWave1 = 1;
				oscParams->wave = BackBeat::WaveType::Triangle;
			}
			else if (strcmp(waveType, "Square") == 0)
			{
				m_OscWave1 = 2;
				oscParams->wave = BackBeat::WaveType::Square;
			}
			else
			{
				BB_CLIENT_ERROR("UNRECOGNIZED WAVETYPE FOR Oscillator1 IN XML FOR {0} TRACK! Loading Sin Wav",
					m_Name.c_str());
				m_OscWave1 = 0;
				oscParams->wave = BackBeat::WaveType::Sin;
			}

			oscParams->amp = oscNode.child("Amp").attribute("Value").as_float();
		}

		// Oscillator 2
		{
			std::shared_ptr<BackBeat::OscParameters> oscParams =
				m_SynthParams->engineParams->voiceParams->OscParams2;

			auto oscNode = node->child("Oscillator2");

			m_Octave2 = oscNode.child("Octave").attribute("Value").as_int();
			oscParams->octave = pow(2.0f, (float)m_Octave2);

			auto waveformNode = oscNode.child("Waveform");

			auto waveType = waveformNode.attribute("Type").value();

			if (strcmp(waveType, "Sin") == 0)
			{
				m_OscWave2 = 0;
				oscParams->wave = BackBeat::WaveType::Sin;
			}
			else if (strcmp(waveType, "SawtoothUp") == 0)
			{
				m_OscWave2 = 3;
				oscParams->wave = BackBeat::WaveType::SawtoothUp;
			}
			else if (strcmp(waveType, "SawtoothDown") == 0)
			{
				m_OscWave2 = 4;
				oscParams->wave = BackBeat::WaveType::SawtoothDown;
			}
			else if (strcmp(waveType, "Triangle") == 0)
			{
				m_OscWave2 = 1;
				oscParams->wave = BackBeat::WaveType::Triangle;
			}
			else if (strcmp(waveType, "Square") == 0)
			{
				m_OscWave2 = 2;
				oscParams->wave = BackBeat::WaveType::Square;
			}
			else
			{
				BB_CLIENT_ERROR("UNRECOGNIZED WAVETYPE FOR Oscillator2 IN XML FOR {0} TRACK! Loading Sin Wav",
					m_Name.c_str());
				m_OscWave2 = 0;
				oscParams->wave = BackBeat::WaveType::Sin;
			}

			oscParams->amp = oscNode.child("Amp").attribute("Value").as_float();
		}

		// Oscillator 3
		{
			std::shared_ptr<BackBeat::OscParameters> oscParams =
				m_SynthParams->engineParams->voiceParams->OscParams3;

			auto oscNode = node->child("Oscillator3");

			m_Octave3 = oscNode.child("Octave").attribute("Value").as_int();
			oscParams->octave = pow(2.0f, (float)m_Octave3);

			auto waveformNode = oscNode.child("Waveform");

			auto waveType = waveformNode.attribute("Type").value();

			if (strcmp(waveType, "Sin") == 0)
			{
				m_OscWave3 = 0;
				oscParams->wave = BackBeat::WaveType::Sin;
			}
			else if (strcmp(waveType, "SawtoothUp") == 0)
			{
				m_OscWave3 = 3;
				oscParams->wave = BackBeat::WaveType::SawtoothUp;
			}
			else if (strcmp(waveType, "SawtoothDown") == 0)
			{
				m_OscWave3 = 4;
				oscParams->wave = BackBeat::WaveType::SawtoothDown;
			}
			else if (strcmp(waveType, "Triangle") == 0)
			{
				m_OscWave3 = 1;
				oscParams->wave = BackBeat::WaveType::Triangle;
			}
			else if (strcmp(waveType, "Square") == 0)
			{
				m_OscWave3 = 2;
				oscParams->wave = BackBeat::WaveType::Square;
			}
			else
			{
				BB_CLIENT_ERROR("UNRECOGNIZED WAVETYPE FOR Oscillator3 IN XML FOR {0} TRACK! Loading Sin Wav",
					m_Name.c_str());
				m_OscWave3 = 0;
				oscParams->wave = BackBeat::WaveType::Sin;
			}

			oscParams->amp = oscNode.child("Amp").attribute("Value").as_float();
		}

		// Oscillator 4
		{
			std::shared_ptr<BackBeat::OscParameters> oscParams =
				m_SynthParams->engineParams->voiceParams->OscParams4;

			auto oscNode = node->child("Oscillator4");

			m_Octave4 = oscNode.child("Octave").attribute("Value").as_int();
			oscParams->octave = pow(2.0f, (float)m_Octave4);

			auto waveformNode = oscNode.child("Waveform");

			auto waveType = waveformNode.attribute("Type").value();

			if (strcmp(waveType, "Sin") == 0)
			{
				m_OscWave4 = 0;
				oscParams->wave = BackBeat::WaveType::Sin;
			}
			else if (strcmp(waveType, "SawtoothUp") == 0)
			{
				m_OscWave4 = 3;
				oscParams->wave = BackBeat::WaveType::SawtoothUp;
			}
			else if (strcmp(waveType, "SawtoothDown") == 0)
			{
				m_OscWave4 = 4;
				oscParams->wave = BackBeat::WaveType::SawtoothDown;
			}
			else if (strcmp(waveType, "Triangle") == 0)
			{
				m_OscWave4 = 1;
				oscParams->wave = BackBeat::WaveType::Triangle;
			}
			else if (strcmp(waveType, "Square") == 0)
			{
				m_OscWave4 = 2;
				oscParams->wave = BackBeat::WaveType::Square;
			}
			else
			{
				BB_CLIENT_ERROR("UNRECOGNIZED WAVETYPE FOR Oscillator4 IN XML FOR {0} TRACK! Loading Sin Wav",
					m_Name.c_str());
				oscParams->wave = BackBeat::WaveType::Sin;
			}

			oscParams->amp = oscNode.child("Amp").attribute("Value").as_float();
		}

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