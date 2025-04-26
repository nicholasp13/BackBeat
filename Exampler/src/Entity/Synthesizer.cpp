// TODO: - Connect LFO to other oscillators

#include "Synthesizer.h"
namespace Exampler {

	static const int
		s_SinIndex = 0,
		s_TriangleIndex = 1,
		s_SquareIndex = 2,
		s_SawtoothUpIndex = 3,
		s_SawtoothDownIndex = 4;
	static const float s_DummyHeight = 19.0f; // About the height of ImGui::Combo
	static const char* s_DutyCycles[] = { "10%", "25%", "40%", "50%" };
	static const int s_NumDutyCycles = 4;
	static const char* s_WaveTypes[] = { "Sin", "Triangle", "Square", "SawtoothUp", "SawtoothDown" };
	static const int s_NumWaveforms = 5;

	Synthesizer::Synthesizer()
		: 
		m_Open(false), 
		m_KeyboardActive(true),
		m_NoteVelocity(BackBeat::MIDI::MaxVelocity),
		m_Pan(BackBeat::SynthBase::PanDefault),
		m_TrackVolume(1.0f),
		m_LFOWave(s_SinIndex),
		m_OscWave1(s_SawtoothUpIndex),
		m_OscWave2(s_SawtoothUpIndex),
		m_OscWave3(s_SawtoothUpIndex),
		m_OscWave4(s_SawtoothUpIndex),
		m_Octave1(0),
		m_Octave2(0),
		m_Octave3(0),
		m_Octave4(0),
		m_PWM1(3),
		m_PWM2(3),
		m_PWM3(3),
		m_PWM4(3),
		m_RecordingPlayer(nullptr),
		m_RecorderMgr(nullptr),
		m_RecordingMappedTrack(nullptr)
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

	void Synthesizer::ImGuiRender()
	{
		auto synthID = m_Synth.GetID();
		ImGui::PushID(synthID.ToString().c_str());
		unsigned int count = SetCanvasColors();

		RenderCanvasEntity();

		ImGui::PopStyleColor(count);

		if (!m_Open)
		{
			ImGui::PopID();
			return;
		}

		// Renders Actual Entity
		const ImGuiViewport* mainViewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowSize(ImVec2(m_Width, m_Height), ImGuiCond_Once);

		count = SetEntityColors();

		// Synth flags
		ImGuiWindowFlags synthWindowFlags = 0;
		synthWindowFlags |= ImGuiWindowFlags_NoCollapse;
		synthWindowFlags |= ImGuiWindowFlags_MenuBar;
		synthWindowFlags |= ImGuiWindowFlags_NoResize;

		// Creates a label ID for ImGui::Begin() that avoids collision to other ImGui::Begin() calls with the same name
		const std::string hashDivider = "###";
		std::string labelID = m_Name.c_str() + hashDivider + synthID.ToString();
		ImGui::Begin(labelID.c_str(), &m_Open, synthWindowFlags);

		RenderMenubar();

		// Table dimensions
		const float padding = 5.0f;
		const float tableLength = m_Width - padding * 2.0f;

		// Table flags
		ImGuiTableFlags tableFlags = 0;
		tableFlags |= ImGuiTableFlags_RowBg;
		tableFlags |= ImGuiTableFlags_Borders;

		// First row
		{
			const int numColumns = 4;
			ImVec2 position = ImVec2(padding, ImGui::GetCursorPos().y);
			ImGui::SetCursorPos(position);
			ImGui::BeginTable("Row1", numColumns, tableFlags, ImVec2(tableLength, 0.0f), 0.0f);

			RenderGeneralControls();
			RenderVolumePanControls();
			RenderLFO();
			RenderFilters();

			ImGui::EndTable();
		}

		// Second row
		{
			const int numColumns = 3;
			const float columnLength = (tableLength * 0.75f) / 2.0f - 9.0f;
			ImVec2 position = ImVec2(padding, ImGui::GetCursorPos().y);
			ImGui::SetCursorPos(position);
			ImGui::BeginTable("Row2", numColumns, tableFlags, ImVec2(tableLength, 0.0f), 0.0f);
			ImGui::TableSetupColumn("one", ImGuiTableColumnFlags_WidthFixed, columnLength);
			ImGui::TableSetupColumn("two", ImGuiTableColumnFlags_WidthFixed, columnLength - 1.0f);

			RenderEGs();
			RenderNoiseGenerator();

			ImGui::EndTable();
		}

		// Third row
		{
			const int numColumns = 4;
			ImVec2 position = ImVec2(padding, ImGui::GetCursorPos().y);
			ImGui::SetCursorPos(position);
			ImGui::BeginTable("Row3", numColumns, tableFlags, ImVec2(tableLength, 0.0f), 0.0f);

			RenderOscs();

			ImGui::EndTable();
		}

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

		m_RecordingMappedTrack = m_RecorderMgr->AddRecordingMappedTrack(synthID, BackBeat::RecorderType::audio);
		m_RecordingPlayer->LoadTrack(m_RecordingMappedTrack);
		
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
			lfoNode.append_child("Delay").append_attribute("Value") = lfoParams->delay;
		}

		// Low Pass Ladder Filter
		{
			std::shared_ptr<BackBeat::LadderFilterParameters> lowpassParams =
				m_SynthParams->engineParams->voiceParams->LPLadderFilterParams;

			auto lowpassNode = synthNode.append_child("LowPassFilter");

			lowpassNode.append_child("FilterOn").append_attribute("Value") = lowpassParams->isOn;
			lowpassNode.append_child("CutoffFrequency").append_attribute("Value") = lowpassParams->cutoff;
			lowpassNode.append_child("Resonance").append_attribute("Value") = lowpassParams->Q;
			lowpassNode.append_child("BassBoost").append_attribute("Value") = lowpassParams->bassBoostPercent;
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

			ampEGNode.append_child("Tracking").append_attribute("Value") = ampEGParams->tracking;
			ampEGNode.append_child("Attack").append_attribute("Value") = ampEGParams->attackDuration;
			ampEGNode.append_child("Decay").append_attribute("Value") = ampEGParams->decayDuration;
			ampEGNode.append_child("Release").append_attribute("Value") = ampEGParams->releaseDuration;
			ampEGNode.append_child("Sustain").append_attribute("Value") = ampEGParams->sustainValue;
		}

		// Filter Envelope Generator
		{
			std::shared_ptr<BackBeat::EGParameters> filterEGParams =
				m_SynthParams->engineParams->voiceParams->EGParams;

			auto filterEGNode = synthNode.append_child("FilterEnvelopeGenerator");

			filterEGNode.append_child("Tracking").append_attribute("Value") = filterEGParams->tracking;
			filterEGNode.append_child("Attack").append_attribute("Value") = filterEGParams->attackDuration;
			filterEGNode.append_child("Decay").append_attribute("Value") = filterEGParams->decayDuration;
			filterEGNode.append_child("Release").append_attribute("Value") = filterEGParams->releaseDuration;
			filterEGNode.append_child("Sustain").append_attribute("Value") = filterEGParams->sustainValue;
		}

		// Noise Generator
		{
			std::shared_ptr<BackBeat::NoiseGeneratorParameters> noiseGenParams =
				m_SynthParams->engineParams->voiceParams->NoiseGenParams;

			auto noiseGenNode = synthNode.append_child("NoiseGenerator");

			noiseGenNode.append_child("On").append_attribute("Value") = noiseGenParams->isOn;
			noiseGenNode.append_child("Level").append_attribute("Value") = noiseGenParams->level;
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
					waveformNode.append_attribute("DutyCycle") = oscParams->dutyCycle;
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
			oscNode.append_child("Detune").append_attribute("Value") = oscParams->detune;
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
					waveformNode.append_attribute("DutyCycle") = oscParams->dutyCycle;
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
			oscNode.append_child("Detune").append_attribute("Value") = oscParams->detune;
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
					waveformNode.append_attribute("DutyCycle") = oscParams->dutyCycle;
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
			oscNode.append_child("Detune").append_attribute("Value") = oscParams->detune;
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
					waveformNode.append_attribute("DutyCycle") = oscParams->dutyCycle;
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
			oscNode.append_child("Detune").append_attribute("Value") = oscParams->detune;
		}

		// Audio track
		{
			auto trackNode = synthNode.append_child("Track");

			auto volumeNode = trackNode.append_child("Volume");
			volumeNode.append_attribute("Value") = m_TrackVolume;

			std::shared_ptr<BackBeat::Track> track = m_RecordingPlayer->GetTrack();
			if (track)
			{
				std::string trackFilePath = BackBeat::Project::GetActive()->GetConfig().tracksDirectoryPath
					+ m_Name + ".wav";

				if (BackBeat::WAVFileBuilder::BuildWAVFile(track.get(), track->GetStart(), track->GetEnd(), trackFilePath))
					trackNode.append_attribute("FilePath") = trackFilePath;
				else
					trackNode.append_attribute("FilePath") = "";
			}
			else
				trackNode.append_attribute("FilePath") = "";
		}
	}

	// NOTE: - node is the node being read from. This is different to WriteObject() || Might want to specify in
	//       function declaration
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
				m_LFOWave = s_SinIndex;
				lfoParams->wave = BackBeat::WaveType::Sin;
			}
			else if (strcmp(waveType, "SawtoothUp") == 0)
			{
				m_LFOWave = s_SawtoothUpIndex;
				lfoParams->wave = BackBeat::WaveType::SawtoothUp;
			}
			else if (strcmp(waveType, "SawtoothDown") == 0)
			{
				m_LFOWave = s_SawtoothDownIndex;
				lfoParams->wave = BackBeat::WaveType::SawtoothDown;
			}
			else if (strcmp(waveType, "Triangle") == 0)
			{
				m_LFOWave = s_TriangleIndex;
				lfoParams->wave = BackBeat::WaveType::Triangle;
			}
			else if (strcmp(waveType, "Square") == 0)
			{
				m_LFOWave = s_SquareIndex;
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
			lfoParams->delay = lfoNode.child("Delay").attribute("Value").as_float();
		}

		// Low Pass Ladder Filter
		{
			std::shared_ptr<BackBeat::LadderFilterParameters> lowpassParams =
				m_SynthParams->engineParams->voiceParams->LPLadderFilterParams;

			auto lowpassNode = node->child("LowPassFilter");

			lowpassParams->isOn = lowpassNode.child("FilterOn").attribute("Value").as_bool();
			lowpassParams->cutoff = lowpassNode.child("CutoffFrequency").attribute("Value").as_float();
			lowpassParams->Q = lowpassNode.child("Resonance").attribute("Value").as_float();
			lowpassParams->bassBoostPercent = lowpassNode.child("BassBoost").attribute("Value").as_float();
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

			ampEGParams->tracking = ampEGNode.child("Tracking").attribute("Value").as_bool();
			ampEGParams->attackDuration = ampEGNode.child("Attack").attribute("Value").as_float();
			ampEGParams->decayDuration = ampEGNode.child("Decay").attribute("Value").as_float();
			ampEGParams->releaseDuration = ampEGNode.child("Release").attribute("Value").as_float();
			ampEGParams->sustainValue = ampEGNode.child("Sustain").attribute("Value").as_float();
		}

		// Amp Envelope Generator
		{
			std::shared_ptr<BackBeat::EGParameters> filterEGParams =
				m_SynthParams->engineParams->voiceParams->EGParams;

			auto filterEGNode = node->child("FilterEnvelopeGenerator");

			filterEGParams->tracking = filterEGNode.child("Tracking").attribute("Value").as_bool();
			filterEGParams->attackDuration = filterEGNode.child("Attack").attribute("Value").as_float();
			filterEGParams->decayDuration = filterEGNode.child("Decay").attribute("Value").as_float();
			filterEGParams->releaseDuration = filterEGNode.child("Release").attribute("Value").as_float();
			filterEGParams->sustainValue = filterEGNode.child("Sustain").attribute("Value").as_float();
		}

		// Noise Generator
		{
			std::shared_ptr<BackBeat::NoiseGeneratorParameters> noiseGenParams =
				m_SynthParams->engineParams->voiceParams->NoiseGenParams;

			auto noiseGenNode = node->child("NoiseGenerator");

			noiseGenParams->isOn = noiseGenNode.child("On").attribute("Value").as_bool();
			noiseGenParams->level = noiseGenNode.child("Level").attribute("Value").as_float();
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
				m_OscWave1 = s_SinIndex;
				oscParams->wave = BackBeat::WaveType::Sin;
			}
			else if (strcmp(waveType, "SawtoothUp") == 0)
			{
				m_OscWave1 = s_SawtoothUpIndex;
				oscParams->wave = BackBeat::WaveType::SawtoothUp;
			}
			else if (strcmp(waveType, "SawtoothDown") == 0)
			{
				m_OscWave1 = s_SawtoothDownIndex;
				oscParams->wave = BackBeat::WaveType::SawtoothDown;
			}
			else if (strcmp(waveType, "Triangle") == 0)
			{
				m_OscWave1 = s_TriangleIndex;
				oscParams->wave = BackBeat::WaveType::Triangle;
			}
			else if (strcmp(waveType, "Square") == 0)
			{
				// Error for floating point as there's usually some error in floating point percision
				const float errOffset = 0.01f;

				float dutyCycle = waveformNode.attribute("DutyCycle").as_float();

				if (dutyCycle <= 0.10f + errOffset)
				{
					dutyCycle = 0.10f;
					m_PWM1 = 0;
				}
				else if (dutyCycle <= 0.25f + errOffset)
				{
					dutyCycle = 0.25f;
					m_PWM1 = 1;
				}
				else if (dutyCycle <= 0.40f + errOffset)
				{
					dutyCycle = 0.40f;
					m_PWM1 = 2;
				}
				else
				{
					dutyCycle = 0.50f;
					m_PWM1 = 3;
				}

				m_OscWave1 = s_SquareIndex;
				oscParams->wave = BackBeat::WaveType::Square;
				oscParams->dutyCycle = dutyCycle;
			}
			else
			{
				BB_CLIENT_ERROR("UNRECOGNIZED WAVETYPE FOR Oscillator1 IN XML FOR {0} TRACK! Loading Sin Wav",
					m_Name.c_str());
				m_OscWave1 = s_SinIndex;
				oscParams->wave = BackBeat::WaveType::Sin;
			}

			oscParams->amp = oscNode.child("Amp").attribute("Value").as_float();
			oscParams->detune = oscNode.child("Detune").attribute("Value").as_float();
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
				m_OscWave2 = s_SinIndex;
				oscParams->wave = BackBeat::WaveType::Sin;
			}
			else if (strcmp(waveType, "SawtoothUp") == 0)
			{
				m_OscWave2 = s_SawtoothUpIndex;
				oscParams->wave = BackBeat::WaveType::SawtoothUp;
			}
			else if (strcmp(waveType, "SawtoothDown") == 0)
			{
				m_OscWave2 = s_SawtoothDownIndex;
				oscParams->wave = BackBeat::WaveType::SawtoothDown;
			}
			else if (strcmp(waveType, "Triangle") == 0)
			{
				m_OscWave2 = s_TriangleIndex;
				oscParams->wave = BackBeat::WaveType::Triangle;
			}
			else if (strcmp(waveType, "Square") == 0)
			{
				// Error for floating point as there's usually some error in floating point percision
				const float errOffset = 0.01f;

				float dutyCycle = waveformNode.attribute("DutyCycle").as_float();

				if (dutyCycle <= 0.10f + errOffset)
				{
					dutyCycle = 0.10f;
					m_PWM2 = 0;
				}
				else if (dutyCycle <= 0.25f + errOffset)
				{
					dutyCycle = 0.25f;
					m_PWM2 = 1;
				}
				else if (dutyCycle <= 0.40f + errOffset)
				{
					dutyCycle = 0.40f;
					m_PWM2 = 2;
				}
				else
				{
					dutyCycle = 0.50f;
					m_PWM2 = 3;
				}

				m_OscWave2 = s_SquareIndex;
				oscParams->wave = BackBeat::WaveType::Square;
				oscParams->dutyCycle = dutyCycle;
			}
			else
			{
				BB_CLIENT_ERROR("UNRECOGNIZED WAVETYPE FOR Oscillator2 IN XML FOR {0} TRACK! Loading Sin Wav",
					m_Name.c_str());
				m_OscWave2 = s_SinIndex;
				oscParams->wave = BackBeat::WaveType::Sin;
			}

			oscParams->amp = oscNode.child("Amp").attribute("Value").as_float();
			oscParams->detune = oscNode.child("Detune").attribute("Value").as_float();
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
				m_OscWave3 = s_SinIndex;
				oscParams->wave = BackBeat::WaveType::Sin;
			}
			else if (strcmp(waveType, "SawtoothUp") == 0)
			{
				m_OscWave3 = s_SawtoothUpIndex;
				oscParams->wave = BackBeat::WaveType::SawtoothUp;
			}
			else if (strcmp(waveType, "SawtoothDown") == 0)
			{
				m_OscWave3 = s_SawtoothDownIndex;
				oscParams->wave = BackBeat::WaveType::SawtoothDown;
			}
			else if (strcmp(waveType, "Triangle") == 0)
			{
				m_OscWave3 = s_TriangleIndex;
				oscParams->wave = BackBeat::WaveType::Triangle;
			}
			else if (strcmp(waveType, "Square") == 0)
			{
				// Error for floating point as there's usually some error in floating point percision
				const float errOffset = 0.01f;

				float dutyCycle = waveformNode.attribute("DutyCycle").as_float();

				if (dutyCycle <= 0.10f + errOffset)
				{
					dutyCycle = 0.10f;
					m_PWM3 = 0;
				}
				else if (dutyCycle <= 0.25f + errOffset)
				{
					dutyCycle = 0.25f;
					m_PWM3 = 1;
				}
				else if (dutyCycle <= 0.40f + errOffset)
				{
					dutyCycle = 0.40f;
					m_PWM3 = 2;
				}
				else
				{
					dutyCycle = 0.50f;
					m_PWM3 = 3;
				}

				m_OscWave3 = s_SquareIndex;
				oscParams->wave = BackBeat::WaveType::Square;
				oscParams->dutyCycle = dutyCycle;
			}
			else
			{
				BB_CLIENT_ERROR("UNRECOGNIZED WAVETYPE FOR Oscillator3 IN XML FOR {0} TRACK! Loading Sin Wav",
					m_Name.c_str());
				m_OscWave3 = s_SinIndex;
				oscParams->wave = BackBeat::WaveType::Sin;
			}

			oscParams->amp = oscNode.child("Amp").attribute("Value").as_float();
			oscParams->detune = oscNode.child("Detune").attribute("Value").as_float();
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
				m_OscWave4 = s_SinIndex;
				oscParams->wave = BackBeat::WaveType::Sin;
			}
			else if (strcmp(waveType, "SawtoothUp") == 0)
			{
				m_OscWave4 = s_SawtoothUpIndex;
				oscParams->wave = BackBeat::WaveType::SawtoothUp;
			}
			else if (strcmp(waveType, "SawtoothDown") == 0)
			{
				m_OscWave4 = s_SawtoothDownIndex;
				oscParams->wave = BackBeat::WaveType::SawtoothDown;
			}
			else if (strcmp(waveType, "Triangle") == 0)
			{
				m_OscWave4 = s_TriangleIndex;
				oscParams->wave = BackBeat::WaveType::Triangle;
			}
			else if (strcmp(waveType, "Square") == 0)
			{
				// Error for floating point as there's usually some error in floating point percision
				const float errOffset = 0.01f;

				float dutyCycle = waveformNode.attribute("DutyCycle").as_float();

				if (dutyCycle <= 0.10f + errOffset)
				{
					dutyCycle = 0.10f;
					m_PWM4 = 0;
				}
				else if (dutyCycle <= 0.25f + errOffset)
				{
					dutyCycle = 0.25f;
					m_PWM4 = 1;
				}
				else if (dutyCycle <= 0.40f + errOffset)
				{
					dutyCycle = 0.40f;
					m_PWM4 = 2;
				}
				else
				{
					dutyCycle = 0.50f;
					m_PWM4 = 3;
				}

				m_OscWave4 = s_SquareIndex;
				oscParams->wave = BackBeat::WaveType::Square;
				oscParams->dutyCycle = dutyCycle;
			}
			else
			{
				BB_CLIENT_ERROR("UNRECOGNIZED WAVETYPE FOR Oscillator4 IN XML FOR {0} TRACK! Loading Sin Wav",
					m_Name.c_str());
				m_OscWave4 = s_SinIndex;
				oscParams->wave = BackBeat::WaveType::Sin;
			}

			oscParams->amp = oscNode.child("Amp").attribute("Value").as_float();
			oscParams->detune = oscNode.child("Detune").attribute("Value").as_float();
		}

		// Audio track
		{
			auto trackNode = node->child("Track");

			auto volumeNode = trackNode.child("Volume");
			m_TrackVolume = volumeNode.attribute("Value").as_float();
			m_RecordingMappedTrack->SetVolume(m_TrackVolume);

			std::string trackFilePath = trackNode.attribute("FilePath").as_string();

			if (!trackFilePath.empty())
			{
				auto trackToCopy = BackBeat::TrackFactory::BuildTrack(trackFilePath);
				BackBeat::TrackFactory::CopyTrackData(trackToCopy, m_RecordingPlayer->GetTrack());
				m_RecordingPlayer->Reset();
			}
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
				if (ImGui::Button("Record On"))
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
				if (ImGui::Button("Record Off"))
					if (!m_RecorderMgr->IsRecording())
						m_RecorderMgr->SetRecorderInactive(synthID);
			}

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
				}

				if (ImGui::Button("Clear Recording"))
					if (!m_RecorderMgr->IsRecording())
						m_RecorderMgr->ResetRecording(synthID);

			}
			else
			{
				if (ImGui::Button("Play Recording On "))
				{
				}
				if (ImGui::Button("Clear Recording"))
				{
				}
			}

		}

		// Track volume control
		ImGui::Text("Volume"); ImGui::SameLine();
		BackBeat::ImGuiWidgets::ImGuiSeekBarFloat("##Volume", &m_TrackVolume, 1.0f, "", ImGuiSliderFlags(0));
		m_RecordingMappedTrack->SetVolume(m_TrackVolume);
		
		ImGui::Spacing();
		ImGui::PopID();
	}

	void Synthesizer::RenderMenubar()
	{
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

	void Synthesizer::RenderGeneralControls()
	{
		ImGui::TableNextColumn();
		ImGui::SeparatorText("General Controls");

		BackBeat::ImGuiWidgets::HelpMarker("Velocity is the measure of how hard you hit the note this means the higher the velocity the shorter the attack duration");
		ImGui::SameLine();

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
		}

		// Makes sure the knobs are properly aligned
		ImGui::SameLine(); ImGui::Dummy(ImVec2(0.0f, s_DummyHeight));

		ImGuiKnobs::KnobInt("Velocity", &m_NoteVelocity, BackBeat::MIDI::MinVelocity, BackBeat::MIDI::MaxVelocity,
			s_KnobSpeed, s_KnobFormatInt, ImGuiKnobVariant_::ImGuiKnobVariant_Wiper);
	}

	void Synthesizer::RenderVolumePanControls()
	{
		float* volume = &(m_SynthParams->engineParams->volume);
		float* pan = &(m_SynthParams->engineParams->voiceParams->DCAParams->pan);
		 
		ImGui::SameLine();
		ImGuiKnobs::Knob("Volume", volume, 0.0f, 1.0f, s_KnobSpeed, s_KnobFormatFloat, ImGuiKnobVariant_::ImGuiKnobVariant_Wiper);
		
		ImGui::SameLine();

		ImGuiKnobs::Knob("Pan", pan, BackBeat::SynthBase::PanMin, BackBeat::SynthBase::PanMax,
			s_KnobSpeed, s_KnobFormatFloat, ImGuiKnobVariant_::ImGuiKnobVariant_WiperDot);

		if (ImGui::IsItemActive() && ImGui::IsMouseDoubleClicked(0)) 
			*pan = 0.0f;

		ImGui::Spacing();
	}

	void Synthesizer::RenderLFO()
	{
		const float comboWidth = 120.0f;

		ImGui::PushID("LFO1");
		ImGui::TableNextColumn();
		ImGui::SeparatorText("LFO");
		BackBeat::ImGuiWidgets::HelpMarker("Connected directly to Oscillator 1");

		BackBeat::WaveType* wave = &(m_SynthParams->engineParams->voiceParams->LFOParams1->wave);

		ImGui::SameLine();
		ImGui::SetNextItemWidth(comboWidth);

		ImGui::Combo("##Waveform", &m_LFOWave, s_WaveTypes, s_NumWaveforms, s_NumWaveforms);

		switch (m_LFOWave)
		{

			case (s_SinIndex):
			{
				*wave = BackBeat::WaveType::Sin;
				break;
			}

			case (s_TriangleIndex):
			{
				*wave = BackBeat::WaveType::Triangle;
				break;
			}

			case (s_SquareIndex):
			{
				*wave = BackBeat::WaveType::Square;
				break;
			}

			case (s_SawtoothUpIndex):
			{
				*wave = BackBeat::WaveType::SawtoothUp;
				break;
			}

			case (s_SawtoothDownIndex):
			{
				*wave = BackBeat::WaveType::SawtoothDown;
				break;
			}

			default:
			{
				break;
			}

		}

		float* LFOFreq1 = &(m_SynthParams->engineParams->voiceParams->LFOParams1->hertz);
		float* LFOAmp1 = &(m_SynthParams->engineParams->voiceParams->LFOParams1->amp);
		float* LFODelay1 = &(m_SynthParams->engineParams->voiceParams->LFOParams1->delay);

		ImGuiKnobs::Knob("Freq", LFOFreq1, BackBeat::SynthBase::LFOFrequencyMin, BackBeat::SynthBase::LFOFrequencyMax,
			s_KnobSpeed, s_KnobFormatFloat, ImGuiKnobVariant_::ImGuiKnobVariant_Wiper);
		
		if (ImGui::IsItemActive() && ImGui::IsMouseDoubleClicked(0))
			*LFOFreq1 = BackBeat::SynthBase::LFOFrequencyDefault;

		ImGui::SameLine();

		ImGuiKnobs::Knob("Amp", LFOAmp1, 0.0f, 1.0f, s_KnobSpeed, s_KnobFormatFloat, ImGuiKnobVariant_::ImGuiKnobVariant_Wiper);
		
		if (ImGui::IsItemActive() && ImGui::IsMouseDoubleClicked(0))
			*LFOAmp1 = 0.0f;

		ImGui::SameLine();

		ImGuiKnobs::Knob("Delay", LFODelay1, BackBeat::SynthBase::LFODelayMin, BackBeat::SynthBase::LFODelayMax,
			s_KnobSpeed, s_KnobFormatFloat, ImGuiKnobVariant_::ImGuiKnobVariant_Wiper);

		if (ImGui::IsItemActive() && ImGui::IsMouseDoubleClicked(0))
			*LFODelay1 = BackBeat::SynthBase::LFODelayMin;

		ImGui::PopID();
	}

	void Synthesizer::RenderFilters()
	{
		// Low Pass Ladder Filter Controls
		{
			ImGui::PushID("LadderLPFilter");
			ImGui::TableNextColumn();

			ImGui::SeparatorText("Low Pass Filter");

			bool* lpFilterOn = &(m_SynthParams->engineParams->voiceParams->LPLadderFilterParams->isOn);
			float* lpCutoffFreq = &(m_SynthParams->engineParams->voiceParams->LPLadderFilterParams->cutoff);
			float* resonance = &(m_SynthParams->engineParams->voiceParams->LPLadderFilterParams->Q);
			float* bassBoost = &(m_SynthParams->engineParams->voiceParams->LPLadderFilterParams->bassBoostPercent);;

			ImGui::Checkbox("Filter On", lpFilterOn);

			ImGuiKnobs::Knob("Cutoff", lpCutoffFreq, BackBeat::SynthBase::FilterCutoffMin, BackBeat::SynthBase::FilterCutoffMax,
				s_KnobSpeed, s_KnobFormatFloatFreq, ImGuiKnobVariant_::ImGuiKnobVariant_WiperDot);

			if (ImGui::IsItemActive() && ImGui::IsMouseDoubleClicked(0))
				*lpCutoffFreq = BackBeat::SynthBase::FilterCutoffMax;

			ImGui::SameLine();

			ImGuiKnobs::Knob("Res", resonance, BackBeat::SynthBase::LadderFilterQMin, BackBeat::SynthBase::LadderFilterQMax,
				s_KnobSpeed, s_KnobFormatFloat, ImGuiKnobVariant_::ImGuiKnobVariant_Wiper);

			if (ImGui::IsItemActive() && ImGui::IsMouseDoubleClicked(0))
				*resonance = BackBeat::SynthBase::LadderFilterQDefault;

			ImGui::SameLine();

			ImGuiKnobs::Knob("Bass", bassBoost, BackBeat::SynthBase::LadderFilterBassBoostMin, BackBeat::SynthBase::LadderFilterBassBoostMax,
				s_KnobSpeed, s_KnobFormatFloat, ImGuiKnobVariant_::ImGuiKnobVariant_Wiper);

			if (ImGui::IsItemActive() && ImGui::IsMouseDoubleClicked(0))
				*bassBoost = BackBeat::SynthBase::LadderFilterBassBoostMin;

			ImGui::Spacing();
			ImGui::PopID();
		}

		// High Pass Filter Controls
		{
			ImGui::PushID("HPFilter");
			ImGui::TableNextColumn();

			ImGui::SeparatorText("High Pass Filter");

			bool* hpFilterOn = &(m_SynthParams->engineParams->voiceParams->HPFilterParams->isOn);
			float* hpCutoffFreq = &(m_SynthParams->engineParams->voiceParams->HPFilterParams->cutoff);
			
			ImGui::Checkbox("Filter On", hpFilterOn);

			ImGuiKnobs::Knob("Cutoff", hpCutoffFreq, BackBeat::SynthBase::FilterCutoffMin, BackBeat::SynthBase::FilterCutoffMax,
				s_KnobSpeed, s_KnobFormatFloatFreq, ImGuiKnobVariant_::ImGuiKnobVariant_WiperDot);

			if (ImGui::IsItemActive() && ImGui::IsMouseDoubleClicked(0))
				*hpCutoffFreq = BackBeat::SynthBase::FilterCutoffMin;

			ImGui::Spacing();
			ImGui::PopID();
		}
	}

	void Synthesizer::RenderEGs()
	{
		// Amp Envelope Generator Controls
		{
			ImGui::PushID("AmpEG");
			ImGui::TableNextColumn();

			bool* tracking = &(m_SynthParams->engineParams->voiceParams->AmpEGParams->tracking);
			float* attackDuration = &(m_SynthParams->engineParams->voiceParams->AmpEGParams->attackDuration);
			float* decayDuration = &(m_SynthParams->engineParams->voiceParams->AmpEGParams->decayDuration);
			float* releaseDuration = &(m_SynthParams->engineParams->voiceParams->AmpEGParams->releaseDuration);
			float* sustain = &(m_SynthParams->engineParams->voiceParams->AmpEGParams->sustainValue);

			ImGui::SeparatorText("Amp Envelope Generator");

			ImGui::Checkbox("Tracking", tracking);

			ImGuiKnobs::Knob("Attack", attackDuration, BackBeat::SynthBase::EG1AttackTimeMin, BackBeat::SynthBase::EG1AttackTimeMax,
				s_KnobSpeed, s_KnobFormatFloat, ImGuiKnobVariant_::ImGuiKnobVariant_Wiper);
			
			if (ImGui::IsItemActive() && ImGui::IsMouseDoubleClicked(0))
				*attackDuration = BackBeat::SynthBase::EG1AttackTimeDefault;

			ImGui::SameLine();

			ImGuiKnobs::Knob("Decay", decayDuration, BackBeat::SynthBase::EG1DecayTimeMin, BackBeat::SynthBase::EG1DecayTimeMax,
				s_KnobSpeed, s_KnobFormatFloat, ImGuiKnobVariant_::ImGuiKnobVariant_Wiper);

			if (ImGui::IsItemActive() && ImGui::IsMouseDoubleClicked(0))
				*decayDuration = BackBeat::SynthBase::EG1DecayTimeDefault;

			ImGui::SameLine();

			ImGuiKnobs::Knob("Release", releaseDuration, BackBeat::SynthBase::EG1ReleaseTimeMin, BackBeat::SynthBase::EG1ReleaseTimeMax,
				s_KnobSpeed, s_KnobFormatFloat, ImGuiKnobVariant_::ImGuiKnobVariant_Wiper);

			if (ImGui::IsItemActive() && ImGui::IsMouseDoubleClicked(0))
				*releaseDuration = BackBeat::SynthBase::EG1ReleaseTimeDefault;

			ImGui::SameLine();

			ImGuiKnobs::Knob("Sustain", sustain, BackBeat::SynthBase::EG1SustainLevelMin, BackBeat::SynthBase::EG1SustainLevelMax,
				s_KnobSpeed, s_KnobFormatFloat, ImGuiKnobVariant_::ImGuiKnobVariant_Wiper);

			if (ImGui::IsItemActive() && ImGui::IsMouseDoubleClicked(0))
				*sustain = BackBeat::SynthBase::EG1SustainLevelDefault;

			ImGui::Spacing();
			ImGui::PopID();
		}

		// Filter Envelope Generator Controls
		{
			ImGui::PushID("FilterEG");
			ImGui::TableNextColumn();

			bool* tracking = &(m_SynthParams->engineParams->voiceParams->EGParams->tracking);
			float* attackDuration = &(m_SynthParams->engineParams->voiceParams->EGParams->attackDuration);
			float* decayDuration = &(m_SynthParams->engineParams->voiceParams->EGParams->decayDuration);
			float* releaseDuration = &(m_SynthParams->engineParams->voiceParams->EGParams->releaseDuration);
			float* sustain = &(m_SynthParams->engineParams->voiceParams->EGParams->sustainValue);

			ImGui::SeparatorText("Filter Envelope Generator");

			ImGui::Checkbox("Tracking", tracking);

			ImGuiKnobs::Knob("Attack", attackDuration, BackBeat::SynthBase::EG1AttackTimeMin, BackBeat::SynthBase::EG1AttackTimeMax,
				s_KnobSpeed, s_KnobFormatFloat, ImGuiKnobVariant_::ImGuiKnobVariant_Wiper);
			
			if (ImGui::IsItemActive() && ImGui::IsMouseDoubleClicked(0))
				*attackDuration = BackBeat::SynthBase::EG1AttackTimeDefault;

			ImGui::SameLine();

			ImGuiKnobs::Knob("Decay", decayDuration, BackBeat::SynthBase::EG1DecayTimeMin, BackBeat::SynthBase::EG1DecayTimeMax,
				s_KnobSpeed, s_KnobFormatFloat, ImGuiKnobVariant_::ImGuiKnobVariant_Wiper);

			if (ImGui::IsItemActive() && ImGui::IsMouseDoubleClicked(0))
				*decayDuration = BackBeat::SynthBase::EG1DecayTimeDefault;

			ImGui::SameLine();

			ImGuiKnobs::Knob("Release", releaseDuration, BackBeat::SynthBase::EG1ReleaseTimeMin, BackBeat::SynthBase::EG1ReleaseTimeMax,
				s_KnobSpeed, s_KnobFormatFloat, ImGuiKnobVariant_::ImGuiKnobVariant_Wiper);

			if (ImGui::IsItemActive() && ImGui::IsMouseDoubleClicked(0))
				*releaseDuration = BackBeat::SynthBase::EG1ReleaseTimeDefault;

			ImGui::SameLine();

			ImGuiKnobs::Knob("Sustain", sustain, BackBeat::SynthBase::EG1SustainLevelMin, BackBeat::SynthBase::EG1SustainLevelMax,
				s_KnobSpeed, s_KnobFormatFloat, ImGuiKnobVariant_::ImGuiKnobVariant_Wiper);

			if (ImGui::IsItemActive() && ImGui::IsMouseDoubleClicked(0))
				*sustain = BackBeat::SynthBase::EG1SustainLevelDefault;

			ImGui::Spacing();
			ImGui::PopID();
		}
	}

	void Synthesizer::RenderNoiseGenerator()
	{
		ImGui::PushID("NoiseGenerator");
		ImGui::TableNextColumn();

		ImGui::SeparatorText("Noise");

		bool* on = &(m_SynthParams->engineParams->voiceParams->NoiseGenParams->isOn);
		float* level = &(m_SynthParams->engineParams->voiceParams->NoiseGenParams->level);

		ImGui::Checkbox("On", on);

		ImGuiKnobs::Knob("Level", level, BackBeat::SynthBase::NoiseGenMin, BackBeat::SynthBase::NoiseGenMax,
			s_KnobSpeed, s_KnobFormatFloat, ImGuiKnobVariant_::ImGuiKnobVariant_Wiper);

		if (ImGui::IsItemActive() && ImGui::IsMouseDoubleClicked(0))
			*level = BackBeat::SynthBase::NoiseGenDefault;

		ImGui::Spacing();
		ImGui::PopID();
	}

	void Synthesizer::RenderOscs()
	{
		RenderOsc("Oscillator 1", &m_Octave1, m_SynthParams->engineParams->voiceParams->OscParams1,
			&m_OscWave1, &m_PWM1);
		RenderOsc("Oscillator 2", &m_Octave2, m_SynthParams->engineParams->voiceParams->OscParams2,
			&m_OscWave2, &m_PWM2);
		RenderOsc("Oscillator 3", &m_Octave3, m_SynthParams->engineParams->voiceParams->OscParams3,
			&m_OscWave3, &m_PWM3);
		RenderOsc("Oscillator 4", &m_Octave4, m_SynthParams->engineParams->voiceParams->OscParams4,
			&m_OscWave4, &m_PWM4);
	}

	void Synthesizer::RenderOsc(const char* label, int* octave, std::shared_ptr<BackBeat::OscParameters> params,
		int* waveIndex, int* PWMIndex)
	{
		const float comboDutyCycleWidth = 60.0f;
		const float comboWidth = 120.0f;

		ImGui::PushID(label);
		ImGui::TableNextColumn();
		ImGui::SeparatorText(label);

		ImGui::Text("Octave: %d", *octave); ImGui::SameLine();
		if (ImGui::SmallButton("+"))
		{
			if (*octave < 2)
				(*octave)++;
		} ImGui::SameLine();
		if (ImGui::SmallButton("-"))
		{
			if (*octave > -2)
				(*octave)--;
		}
		params->octave = pow(2.0f, (float)*octave);

		BackBeat::WaveType* wave = &(params->wave);

		ImGui::SetNextItemWidth(comboWidth);

		ImGui::Combo("##Waveform", waveIndex, s_WaveTypes, s_NumWaveforms, s_NumWaveforms);

		switch (*waveIndex)
		{

			case (s_SinIndex):
			{
				*wave = BackBeat::WaveType::Sin;
				break;
			}

			case (s_TriangleIndex):
			{
				*wave = BackBeat::WaveType::Triangle;
				break;
			}

			case (s_SquareIndex):
			{
				*wave = BackBeat::WaveType::Square;
				break;
			}

			case (s_SawtoothUpIndex):
			{
				*wave = BackBeat::WaveType::SawtoothUp;
				break;
			}

			case (s_SawtoothDownIndex):
			{
				*wave = BackBeat::WaveType::SawtoothDown;
				break;
			}

			default:
			{
				break;
			}

		}

		// Square Wave Pulse Width Modulation
		if (*wave == BackBeat::WaveType::Square)
		{
			ImGui::SetNextItemWidth(comboDutyCycleWidth);
			ImGui::Combo("##DutyCycle", PWMIndex, s_DutyCycles, s_NumDutyCycles, s_NumDutyCycles);
			ImGui::SameLine();
			BackBeat::ImGuiWidgets::HelpMarker("Square Wave Pulse Width Modulation (PWM) \nChanges the length of the active duty cycle of the wave by the percentage");

			switch (*PWMIndex)
			{

			case (0):
			{
				params->dutyCycle = 0.10f;
				break;
			}

			case (1):
			{
				params->dutyCycle = 0.25f;
				break;
			}

			case (2):
			{
				params->dutyCycle = 0.40f;
				break;
			}

			case (3):
			{
				params->dutyCycle = 0.50f;
				break;
			}

			}

			ImGui::SameLine();
		}

		ImGui::Dummy(ImVec2(0.0f, s_DummyHeight));

		float* detune = &(params->detune);
		float* waveAmp = &(params->amp);

		ImGuiKnobs::Knob("Detune", detune, BackBeat::SynthBase::WaveDetuneMin, BackBeat::SynthBase::WaveDetuneMax,
			s_KnobSpeed, s_KnobFormatFloat, ImGuiKnobVariant_::ImGuiKnobVariant_WiperDot);

		if (ImGui::IsItemActive() && ImGui::IsMouseDoubleClicked(0))
			*detune = BackBeat::SynthBase::WaveDetuneDefault;

		ImGui::SameLine();

		ImGuiKnobs::Knob("Amp", waveAmp, 0.0f, 1.0f, s_KnobSpeed, s_KnobFormatFloat, ImGuiKnobVariant_::ImGuiKnobVariant_Wiper);


		ImGui::Spacing();

		ImGui::PopID();
	}

	unsigned int Synthesizer::SetCanvasColors()
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

	unsigned int Synthesizer::SetEntityColors()
	{
		unsigned int count = 0;

		// MenuBar colors
		ImGui::PushStyleColor(ImGuiCol_MenuBarBg, IM_COL32(24, 29, 38, 255)); count++;

		// Window colors
		ImGui::PushStyleColor(ImGuiCol_Border, IM_COL32(91, 99, 115, 255)); count++;
		ImGui::PushStyleColor(ImGuiCol_WindowBg, IM_COL32(91, 99, 115, 255)); count++;

		// Table colors
		ImGui::PushStyleColor(ImGuiCol_TableRowBg, IM_COL32(24, 29, 38, 255)); count++;
		ImGui::PushStyleColor(ImGuiCol_TableRowBgAlt, IM_COL32(24, 29, 38, 255)); count++;
		ImGui::PushStyleColor(ImGuiCol_TableBorderStrong, IM_COL32(91, 99, 115, 255)); count++;
		ImGui::PushStyleColor(ImGuiCol_TableBorderLight, IM_COL32(91, 99, 115, 255)); count++;

		// Misc colors
		ImGui::PushStyleColor(ImGuiCol_Separator, IM_COL32(91, 99, 115, 255)); count++;

		// Knobs colors
		// Filled
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImGui::GetColorU32(ImGuiCol_SliderGrab)); count++;
		// Filled (Hovered)
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, IM_COL32(0, 80, 255, 255)); count++;
		// Track
		ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(44, 49, 56, 255)); count++;

		return count;
	}

}