// TODO: - Create ImGui Pad widget and Pad control popup/window
//       - Fix panning volume level

// NOTE: Current panning settings currently make the sample quieter compared to when the sample splicer makes them.

#include "Sampler.h"
namespace Exampler {

	Sampler::Sampler()
		:
		m_Open(false),
		m_KeyboardActive(true),
		m_CreatingSample(false),
		m_ProgrammingNote(false),
		m_DevicesOpen(0),
		m_NumMIDIDevices(0),
		m_PadToProgram(0),
		m_RecordingPlayer(nullptr),
		m_RecorderMgr(nullptr)
	{
		m_NumPads = m_Sampler.GetProgrammer()->GetNumPads();
	}

	Sampler::~Sampler()
	{
		m_Sampler.Stop();
		m_RecordingPlayer->Stop();
	}

	void Sampler::Update()
	{
		
	}

	void Sampler::OnEvent(BackBeat::Event& event)
	{
		BackBeat::EventDispatcher dispatcher(event);

		dispatcher.Dispatch<BackBeat::KeyPressedEvent>(BIND_EVENT_FN(Sampler::OnKeyEvent));
		dispatcher.Dispatch<BackBeat::MouseButtonPressedEvent>(BIND_EVENT_FN(Sampler::OnMouseButtonEvent));

		if (m_Sampler.IsRunning() && m_KeyboardActive)
			event.Handled = m_Sampler.GetEventHandler()->HandleEvent(event);
	}

	void Sampler::ImGuiRender()
	{
		auto samplerID = m_Sampler.GetID();
		unsigned int count = SetSamplerColors();

		ImGui::PushID(samplerID.ToString().c_str());

		RenderCanvasEntity();

		if (!m_Open)
		{
			ImGui::PopStyleColor(count);
			ImGui::PopID();
			return;
		}

		const ImGuiViewport* mainViewport = ImGui::GetMainViewport();
		const float width = 1000.0f;
		const float height = 600.0f;
		float x = mainViewport->WorkPos.x;
		float y = mainViewport->WorkPos.y;
		ImGui::SetNextWindowPos(ImVec2(x, y), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize(ImVec2(width, height), ImGuiCond_Once);
		// Sampler flags
		ImGuiWindowFlags sampler_window_flags = 0;
		sampler_window_flags |= ImGuiWindowFlags_NoCollapse;
		sampler_window_flags |= ImGuiWindowFlags_MenuBar;
		sampler_window_flags |= ImGuiWindowFlags_NoResize;

		// Creates a label ID for ImGui::Begin() that avoids collision to other ImGui::Begin() calls with the same name
		const std::string hashDivider = "###";
		std::string labelID = m_Name.c_str() + hashDivider + samplerID.ToString();
		ImGui::Begin(labelID.c_str(), &m_Open, sampler_window_flags);

		RenderMenuBar();
		RenderSamplerPads();
		// PopUps
		RenderSampleCreator();
		RenderNoteProgrammer();

		ImGui::End();
		ImGui::PopStyleColor(count);
		ImGui::PopID();
	}

	void Sampler::Add(
		BackBeat::PlayerManager* playerMgr,
		BackBeat::RecorderManager* recorderMgr,
		BackBeat::Mixer* mixer,
		BackBeat::MIDIDeviceManager* midiDeviceManager)
	{
		m_RecorderMgr = recorderMgr;
		m_RecordingPlayer = playerMgr->AddNewPlayer();
		auto samplerProc = m_Sampler.GetProcessor();
		auto samplerID = samplerProc->GetID();
		auto id = m_Sampler.GetID();
		auto samplerTrack = m_RecorderMgr->AddRecordingTrack(samplerID, BackBeat::RecorderType::audio);
		m_RecordingPlayer->LoadTrack(samplerTrack);

		mixer->PushProcessor(samplerProc);
		mixer->PushProcessor(m_RecordingPlayer->GetProc());
		mixer->PushProcessor(m_TrackPlayer.GetProc());
		midiDeviceManager->PushOutput(m_Sampler.GetMIDIInput());
	}

	void Sampler::Delete(
		BackBeat::PlayerManager* playerMgr,
		BackBeat::RecorderManager* recorderMgr,
		BackBeat::Mixer* mixer,
		BackBeat::MIDIDeviceManager* midiDeviceManager)
	{
		auto samplerID = m_Sampler.GetID();
		auto trackPlayerID = m_RecordingPlayer->GetID();
		auto midiInputID = m_Sampler.GetMIDIInput()->GetID();
		auto sampleTrackPlayerID = m_TrackPlayer.GetID();

		m_Sampler.Stop();
		m_RecordingPlayer->Stop();

		playerMgr->Delete(m_RecordingPlayer->GetID());
		recorderMgr->DeleteTrack(samplerID);
		mixer->DeleteProcessor(samplerID);
		mixer->DeleteProcessor(trackPlayerID);
		mixer->DeleteProcessor(sampleTrackPlayerID);
		midiDeviceManager->DeleteOutput(midiInputID);
	}

	void Sampler::RenderCanvasEntity()
	{
		auto samplerID = m_Sampler.GetID();

		ImGui::PushID(samplerID.ToString().c_str());
		ImGui::Spacing();
		ImGui::SeparatorText(m_Name.c_str());

		if (!m_Sampler.IsRunning())
		{
			if (ImGui::Button("On "))
				On();
		}
		else
		{
			if (ImGui::Button("Off"))
			{
				m_RecorderMgr->SetRecorderInactive(samplerID);
				Off();
			}
		}
		ImGui::SameLine();

		if (ImGui::Button("Open"))
			Open();

		// Render Recorder controls
		{
			if (!m_RecorderMgr->IsActive(samplerID))
			{
				if (ImGui::Button("Record On", ImVec2(125, 20)))
				{
					if (!m_RecorderMgr->IsRecording())
					{
						On();
						m_RecorderMgr->SetRecorderActive(samplerID);		
					}
				}
			}
			else
			{
				if (ImGui::Button("Record Off", ImVec2(125, 20)))
					if (!m_RecorderMgr->IsRecording())
						m_RecorderMgr->SetRecorderInactive(samplerID);
			}
			ImGui::SameLine();

		}

		// Render Recording Track Player controls
		{
			if (m_RecordingPlayer && m_RecordingPlayer->GetSize() > 0)
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
					if (!m_RecorderMgr->IsActive(samplerID))
						m_RecorderMgr->ResetRecording(samplerID);


				BackBeat::TimeMinSec trackTime = m_RecordingPlayer->GetTime();
				BackBeat::TimeMinSec trackLength = m_RecordingPlayer->GetLength();

				int position = m_RecordingPlayer->GetPosition();
				int size = m_RecordingPlayer->GetSize();
				static bool wasPlaying = false;
				ImGui::Text("%d:%02d", trackTime.minutes, trackTime.seconds); ImGui::SameLine();

				// Placeholder for future implementation of a custom ImGui::Timeline widget
				ImGui::PushID("Seekbar");
				if (BackBeat::ImGuiWidgets::ImGuiSeekBarInt("##", &position, m_RecordingPlayer->GetSize(), "", ImGuiSliderFlags(0)))
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

		float* volume = &(m_Sampler.GetEngineParams()->volume);
		ImGui::Text("    "); ImGui::SameLine();
		BackBeat::ImGuiWidgets::ImGuiSeekBarFloat("Volume", volume, 1.0f, "", ImGuiSliderFlags(0));

		ImGui::Spacing();
		ImGui::PopID();
	}

	void Sampler::RenderMenuBar()
	{
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Menu"))
			{
				if (ImGui::MenuItem("Create Sample"))
				{
					m_CreatingSample = true;
					m_TrackPlayer.Start();
				}

				if (ImGui::BeginMenu("Program Pad"))
				{
					char label[10] = {};
					for (unsigned int i = 1; i <= m_NumPads; i++) {
						sprintf_s(label, "Pad #%d", i);
						if (ImGui::BeginMenu(label))
						{
							if (ImGui::MenuItem("Set Note"))
							{
								m_ProgrammingNote = true;
								m_PadToProgram = i;
							}

							if (ImGui::MenuItem("Set Sample"))
							{
								m_Sampler.GetProgrammer()->ProgramSample(i - 1);
							}

							ImGui::EndMenu();
						}
					}

					ImGui::EndMenu();
				}

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Devices"))
			{
				// Keyboard
				if (ImGui::BeginMenu("Keyboard"))
				{
					if (ImGui::MenuItem("Set Active", "", &m_KeyboardActive))
					{

					}

					ImGui::EndMenu();
				}

				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

	}

	// TODO: - Make each pad its own large square button/widget that left click plays the sample and right click opens
	//         a menu to program the pad
	void Sampler::RenderSamplerPads()
	{
		auto sampleProgrammer = m_Sampler.GetProgrammer();
		const unsigned int charLimit = 80;
		static char sampleNameLabel[charLimit];
		static std::string sampleName = "";
		static char padNameLable[charLimit];
		static char loopID[charLimit];
		int padCode = 0;

		ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();

		static ImGuiTableFlags table_flags = 0;
		table_flags |= ImGuiTableFlags_RowBg;
		table_flags |= ImGuiTableFlags_BordersH;
		table_flags |= ImGuiTableFlags_BordersV;
		ImGui::BeginTable("SampleEditor", 4, table_flags, ImVec2(0.0f, 0.0f), 0.0f);

		for (unsigned int i = 0; i < m_NumPads; i++) {
			ImGui::TableNextColumn();

			sprintf_s(padNameLable, "PAD #%d", i + 1);
			ImGui::PushID(padNameLable);

			ImGui::SeparatorText(padNameLable);
			ImGui::Spacing();

			sampleName = sampleProgrammer->GetPadName(i);
			padCode = (int)sampleProgrammer->GetPadNote(i);
			strcpy_s(sampleNameLabel, sampleName.c_str());

			ImGui::Text("   NAME:"); ImGui::SameLine();
			ImGui::Text(sampleNameLabel);

			// Keybinds
			const unsigned int numCodes = 10;
			if (padCode == BackBeat::MIDI::NoteOff)
				ImGui::Text("   NOTE: NOTE OFF");
			else
			{
				ImGui::Text("   NOTE:"); ImGui::SameLine();
				ImGui::Text(BackBeat::MIDI::MIDINoteNames[padCode]);
			}
			if (i < numCodes - 1)
				ImGui::Text("KEYBIND: %d", i + 1);
			else if (i == numCodes - 1)
				ImGui::Text("KEYBIND: 0");
			else
				ImGui::Text("KEYBIND: F%d", i - numCodes + 1);

			ImGui::Spacing();

			// Loop
			bool looping = sampleProgrammer->IsLooping(i);
			if (ImGui::Checkbox("Loop", &looping))
			{
				if (looping)
					sampleProgrammer->LoopOn(i);
				else
					sampleProgrammer->LoopOff(i);
			}
			ImGui::Spacing();

			// Volume controls
			float* volume = &(sampleProgrammer->GetSamplePad(i)->GetDCAParameters()->volume);
			ImGui::Text("    "); ImGui::SameLine(); ImGui::SliderFloat("Volume", volume, 0.0f, 1.0f);
			ImGui::Spacing();

			// Sample Pad DCA/panning controls
			const float defaultAmp = 0.5f;
			float pan = (sampleProgrammer->GetSamplePad(i)->GetDCAParameters()->rightAmp -
				sampleProgrammer->GetSamplePad(i)->GetDCAParameters()->leftAmp) / 2;
			ImGui::Text("Panning"); ImGui::SameLine();
			if (ImGui::SmallButton("Reset"))
				pan = 0.0f;
			ImGui::Text("Left"); ImGui::SameLine(); ImGui::SliderFloat("Right", &pan, BackBeat::SynthBase::PanMin, BackBeat::SynthBase::PanMax);
			sampleProgrammer->GetSamplePad(i)->GetDCAParameters()->leftAmp = defaultAmp - pan;
			sampleProgrammer->GetSamplePad(i)->GetDCAParameters()->rightAmp = defaultAmp + pan;

			ImGui::PopID();
			ImGui::Spacing();
		}

		ImGui::EndTable();
	}

	void Sampler::RenderSampleCreator()
	{
		const float width = 900.0f;
		const float height = 200.0f;
		ImGui::SetNextWindowSize(ImVec2(width, height));

		const unsigned int charLimit = 80;
		static char trackName[charLimit];
		static std::string sampleName = "";
		static bool trackSet = false;
		static bool isLooping = false;
		static int zero = 0;
		static int start = 0;
		static int end = 0;
		static int size = 0;
		static int startMs = 0;
		static int endMs = 0;
		static int bytesPerMs = 0;
		float byteRate = 0.0f;

		if (m_CreatingSample)
		{
			ImGui::OpenPopup(CreatingSampleID);
			m_CreatingSample = false;
		}

		ImGui::PushStyleColor(ImGuiCol_PopupBg, IM_COL32(147, 157, 169, 255));
		if (ImGui::BeginPopup(CreatingSampleID))
		{
			ImGui::SeparatorText("Sample Creator");

			// Open button
			{
				ImGui::PushID("Open");

				if (ImGui::Button("Open"))
				{
					m_TrackPlayer.Pause();
					m_TrackPlayer.LoadTrack(BackBeat::FileDialog::OpenFile("WAV Files (*.wav)\0*.wav\0"));
					strcpy_s(trackName, m_TrackPlayer.GetTrackName().c_str());
					trackSet = m_TrackPlayer.IsLoaded();
					zero = 0;
					start = 0;
					end = m_TrackPlayer.GetSize();
					size = end;
					bytesPerMs = m_TrackPlayer.GetByteRate() / 1000;
					startMs = 0;
					endMs = (int)((float)end / (float)bytesPerMs);
				}
				ImGui::SameLine();
				ImGui::PopID();
			}

			// Save button
			{
				ImGui::PushID("Save");

				if (ImGui::Button("Save"))
				{
					BackBeat::SampleBuilder::SaveSample(m_TrackPlayer.GetTrack(), start, end);
					ImGui::CloseCurrentPopup();
				}
				ImGui::SameLine();
				ImGui::PopID();
			}

			// Clear button
			{
				ImGui::PushID("Clear");

				if (ImGui::Button("Clear"))
				{
					m_TrackPlayer.ClearTrack();
					strcpy_s(trackName, "");
					zero = 0;
					start = 0;
					end = 0;
					size = 0;
					bytesPerMs = 0;
					startMs = 0;
					endMs = 0;
					m_CreatingSample = false;
					trackSet = false;
				}

				ImGui::PopID();
			}

			// Track Name
			ImGui::Text("Track: "); ImGui::SameLine(); ImGui::Text(trackName);

			// Renders Track Editor
			{
				ImGui::PushID("SampleEditor");
				ImGui::PushItemWidth(width - 200.0f);
				byteRate = (float)m_TrackPlayer.GetByteRate();
				BackBeat::TimeMinSec zeroTime = BackBeat::TimeMinSec();
				BackBeat::TimeMinSec sizeTime = BackBeat::TimeMinSec();;
				if (m_TrackPlayer.IsLoaded())
				{
					zeroTime = BackBeat::Audio::GetTime((float)zero / byteRate);
					sizeTime = BackBeat::Audio::GetTime((float)size / byteRate);
				}
				else
				{
					zeroTime.minutes = 0;
					zeroTime.seconds = 0;
					sizeTime.minutes = 0;
					sizeTime.seconds = 0;
				}
				ImGui::Text("%d:%02d", zeroTime.minutes, zeroTime.seconds); ImGui::SameLine();
				if (BackBeat::ImGuiWidgets::ImGuiTrackEditor("##", &start, &end, &zero, &size, "", ImGuiSliderFlags(0)))
				{
					m_TrackPlayer.Pause();
				}
				ImGui::SameLine(); ImGui::Text("%d:%02d", sizeTime.minutes, sizeTime.seconds);
				ImGui::PopID();
				ImGui::Spacing();
			}

			// Renders volume seekbar
			{
				ImGui::PushID("Volume");
				ImGui::PushItemWidth(width - 200.0f);
				static float volume = 1.0f;
				ImGui::Text("    "); ImGui::SameLine();
				BackBeat::ImGuiWidgets::ImGuiSeekBarFloat("Volume", &volume, 1.0f, "", ImGuiSliderFlags(0));
				m_TrackPlayer.SetVolume(volume);
				ImGui::PopID();
				ImGui::Spacing();
			}

			// Renders Editor Buttons
			{
				ImGui::PushID("Play");
				if (!m_TrackPlayer.IsPlaying())
				{
					if (ImGui::Button("Play"))
					{
						m_TrackPlayer.SetPosition(start);
						m_TrackPlayer.On();
						m_TrackPlayer.Start();
					}
				}
				else if (m_TrackPlayer.IsLoaded())
				{
					if (ImGui::Button("Stop"))
						m_TrackPlayer.Pause();
				}
				ImGui::PopID();
				ImGui::SameLine();

				// Loop
				ImGui::PushID("Loop");
				ImGui::Checkbox("Loop", &isLooping);
				ImGui::PopID();
				ImGui::SameLine();

				// Zoom In
				int padding = bytesPerMs;
				ImGui::PushID("ZoomIn");
				if (ImGui::Button("Zoom in"))
				{
					if (trackSet && (end - start >= padding))
					{
						zero = start;
						size = end;
					}
				}
				ImGui::PopID();
				ImGui::SameLine();

				// Zoom Out
				ImGui::PushID("ZoomOut");
				if (ImGui::Button("Zoom Out"))
				{
					if (trackSet)
					{
						int zoomIncrement = (int)byteRate;
						int trackSize = (int)m_TrackPlayer.GetSize();
						zero = zoomIncrement > zero
							? 0 : (zero - zoomIncrement);
						size = (size + zoomIncrement) > trackSize
							? trackSize : (size + zoomIncrement);
					}
				}
				ImGui::PopID();
				ImGui::SameLine();

				// Zoom Reset
				ImGui::PushID("ResetZoom");
				if (ImGui::Button("Reset Zoom"))
				{
					if (trackSet)
					{
						zero = 0;
						size = m_TrackPlayer.GetSize();
					}
				}
				ImGui::PopID();
			}

			static ImGuiTableFlags table_flags = 0;
			ImGui::BeginTable("SampleEditor", 2, table_flags, ImVec2(0.0f, 0.0f), 0.0f);
			const unsigned int incrementMs = 10;
			// Renders Start Input
			{
				ImGui::TableNextColumn();
				ImGui::PushID("Start");

				float byteRate = (float)m_TrackPlayer.GetByteRate();
				BackBeat::TimeMinSec startTime = BackBeat::TimeMinSec();
				if (m_TrackPlayer.IsLoaded())
				{
					startTime = BackBeat::Audio::GetTime((float)start / byteRate);
				}
				else
				{
					startTime.minutes = 0;
					startTime.seconds = 0;
				}

				ImGui::Text("Start: %d:%02d", startTime.minutes, startTime.seconds); ImGui::SameLine();
				if (trackSet)
				{
					startMs = (int)((float)start / (float)bytesPerMs);
					if (ImGui::InputInt("(in ms)", &startMs, incrementMs, incrementMs * 10, ImGuiInputTextFlags(0)))
					{
						start = startMs < (endMs - (int)incrementMs) ? (startMs * bytesPerMs) : (endMs - (int)incrementMs);
					}
				}
				else
				{
					int dummyStartZero = 0;
					ImGui::InputInt("(in ms)", &dummyStartZero);
					dummyStartZero = 0;
				}
				ImGui::PopID();
			}

			// Renders End Input
			{
				ImGui::TableNextColumn();
				ImGui::PushID("End");

				BackBeat::TimeMinSec endTime = BackBeat::TimeMinSec();
				if (m_TrackPlayer.IsLoaded())
				{
					endTime = BackBeat::Audio::GetTime((float)end / byteRate);
				}
				else
				{
					endTime.minutes = 0;
					endTime.seconds = 0;
				}

				ImGui::Text("End: %d:%02d", endTime.minutes, endTime.seconds); ImGui::SameLine();
				if (trackSet)
				{
					endMs = (int)((float)end / (float)bytesPerMs);
					if (ImGui::InputInt("(in ms)", &endMs, incrementMs, incrementMs * 10, ImGuiInputTextFlags(0)))
					{
						end = endMs > (startMs + (int)incrementMs) ? (endMs * bytesPerMs) : (startMs + (int)incrementMs);
					}
				}
				else
				{
					int dummyEndZero = 0;
					ImGui::InputInt("(in ms)", &dummyEndZero);
					dummyEndZero = 0;
				}
				ImGui::PopID();
			}

			// Total Time
			{
				ImGui::TableNextColumn();
				ImGui::PushID("TotalTime");

				BackBeat::TimeMinSec totalTime = BackBeat::TimeMinSec();
				if (m_TrackPlayer.IsLoaded())
				{
					float byteRate = (float)m_TrackPlayer.GetByteRate();
					totalTime = BackBeat::Audio::GetTime((float)(end - start) / byteRate);
					totalTime.milliseconds = endMs - startMs;
				}
				else
				{
					totalTime.minutes = 0;
					totalTime.seconds = 0;
					totalTime.milliseconds = 0;
				}
				ImGui::Text("Total: %d:%02d (%d ms)", totalTime.minutes, totalTime.seconds, totalTime.milliseconds);

				ImGui::PopID();
			}
			ImGui::EndTable();

			// Set Start and End values
			if (m_TrackPlayer.IsLoaded())
			{
				m_TrackPlayer.SetStart(start);
				m_TrackPlayer.SetEnd(end);
				if ((int)m_TrackPlayer.GetPosition() >= end)
				{
					if (isLooping)
						m_TrackPlayer.SetPosition(start);
					else
						m_TrackPlayer.Pause();
				}
			}

			ImGui::EndPopup();
		}
		ImGui::PopStyleColor(1);

		if (!ImGui::IsPopupOpen(CreatingSampleID))
		{
			m_TrackPlayer.Stop();
			m_TrackPlayer.ClearTrack();
			strcpy_s(trackName, "");
			zero = 0;
			start = 0;
			end = 0;
			size = 0;
			bytesPerMs = 0;
			startMs = 0;
			endMs = 0;
			m_CreatingSample = false;
			trackSet = false;
		}
	}

	void Sampler::RenderNoteProgrammer()
	{
		static int newCode = 0;

		if (m_ProgrammingNote)
		{
			ImGui::OpenPopup(ProgrammingNoteID);
			m_ProgrammingNote = false;
		}
		if (ImGui::BeginPopup(ProgrammingNoteID))
		{
			int code = (int)m_Sampler.GetProgrammer()->GetPadNote(m_PadToProgram - 1);
			const unsigned int charLimit = 80;
			static char padNameLabel[charLimit];
			static std::string padName = "";

			sprintf_s(padNameLabel, "Pad #%d", m_PadToProgram);

			ImGui::SeparatorText(padNameLabel);

			ImGui::Text("Current Note:"); ImGui::SameLine();
			if (code == (int)BackBeat::MIDI::NoteOff)
				ImGui::Text("OFF");
			else
				ImGui::Text(BackBeat::MIDI::MIDINoteNames[code]);

			ImGui::Combo("Notes", &newCode, BackBeat::MIDI::MIDINoteNames, (int)BackBeat::MIDI::NumMIDINotes);

			if (ImGui::SmallButton("Save"))
			{
				m_Sampler.GetProgrammer()->ProgramNote(m_PadToProgram - 1, (BackBeat::MIDICode)newCode);
				ImGui::CloseCurrentPopup();
			}
			ImGui::SameLine();

			if (ImGui::SmallButton("Clear"))
			{
				m_Sampler.GetProgrammer()->ClearNote(m_PadToProgram - 1);
			}
			ImGui::SameLine();

			// MIDI Input Popup
			if (ImGui::SmallButton("Press MIDI Key"))
			{
				ImGui::OpenPopup("MIDIKeyInput");
				m_Sampler.GetMIDIInput()->ClearEvent();
			}
			if (ImGui::BeginPopup("MIDIKeyInput"))
			{
				ImGui::Text("Press MIDI Key");

				if (m_Sampler.GetMIDIInput()->IsKeyPresssed())
				{
					newCode = (int)m_Sampler.GetMIDIInput()->GetLastKeyPressed();
					ImGui::CloseCurrentPopup();
				}

				ImGui::EndPopup();
			}
			ImGui::EndPopup();
		}

		if (!ImGui::IsPopupOpen(ProgrammingNoteID))
		{
			newCode = 0;
		}
	}

	unsigned int Sampler::SetSamplerColors()
	{
		unsigned int count = 0;

		// MenuBar colors
		ImGui::PushStyleColor(ImGuiCol_MenuBarBg, IM_COL32(91, 115, 109, 255)); count++;

		// Window colors
		ImGui::PushStyleColor(ImGuiCol_Border, IM_COL32(91, 115, 109, 255)); count++;

		// Table colors
		ImGui::PushStyleColor(ImGuiCol_TableRowBg, IM_COL32(24, 38, 39, 255)); count++;
		ImGui::PushStyleColor(ImGuiCol_TableRowBgAlt, IM_COL32(24, 38, 39, 255)); count++;
		ImGui::PushStyleColor(ImGuiCol_TableBorderStrong, IM_COL32(44, 56, 59, 255)); count++;
		ImGui::PushStyleColor(ImGuiCol_TableBorderLight, IM_COL32(44, 56, 59, 255)); count++;

		// Misc colors
		ImGui::PushStyleColor(ImGuiCol_Separator, IM_COL32(99, 115, 101, 255)); count++;
		ImGui::PushStyleColor(ImGuiCol_PlotHistogram, IM_COL32(61, 224, 144, 255)); count++;
		ImGui::PushStyleColor(ImGuiCol_SliderGrab, IM_COL32(61, 224, 144, 255)); count++;
		ImGui::PushStyleColor(ImGuiCol_SliderGrabActive, IM_COL32(61, 224, 144, 255)); count++;

		return count;
	}
}