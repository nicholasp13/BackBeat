// TODO: - Redesign GUI
//           - Add ImGui::Timeline to show waveform/data
//       - Switch to MappedTrack
//       - Fix bug when sampling MONO tracks
//       - Add zero crossing finding tool

#include "SampleSplicer.h"
namespace Exampler {

	SampleSplicer::SampleSplicer()
		: 
		m_Open(false),
		m_Looping(false),
		m_Zero(0),
		m_Start(0),
		m_End(0),
		m_Size(0),
		m_StartMs(0),
		m_EndMs(0),
		m_Volume(1.0f)
	{

	}

	SampleSplicer::~SampleSplicer()
	{
		m_TrackPlayer.Stop();
	}

	void SampleSplicer::Update()
	{

	}

	void SampleSplicer::Render()
	{
		if (!m_Open)
			return;

		const unsigned int charLimit = 80;
		char trackName[charLimit] = {};
		std::string sampleName = "";
		bool trackSet = m_TrackPlayer.IsLoaded();
		int bytesPerMs = 0;
		float byteRate = 0.0f;

		if (trackSet)
			bytesPerMs = m_TrackPlayer.GetByteRate() / 1000;

		const ImGuiViewport* mainViewport = ImGui::GetMainViewport();
		float x = mainViewport->WorkPos.x;
		float y = mainViewport->WorkPos.y;
		ImGui::SetNextWindowPos(ImVec2(x, y), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize(ImVec2(m_Width, m_Height), ImGuiCond_Once);
		// Sampler flags
		ImGuiWindowFlags splicerWindowFlags = 0;
		splicerWindowFlags |= ImGuiWindowFlags_NoCollapse;
		splicerWindowFlags |= ImGuiWindowFlags_NoResize;

		if (ImGui::Begin("Sample Splicer", &m_Open, splicerWindowFlags))
		{
			ImGui::PushID(m_TrackPlayer.GetID().ToString().c_str());

			// Open button
			{
				if (ImGui::Button("Open"))
				{
					m_TrackPlayer.Pause();
					m_TrackPlayer.LoadTrack(BackBeat::FileDialog::OpenFile("WAV Files (*.wav)\0*.wav\0"));
					strcpy_s(trackName, m_TrackPlayer.GetTrackName().c_str());
					trackSet = m_TrackPlayer.IsLoaded();
					m_Zero = 0;
					m_Start = 0;
					m_End = m_TrackPlayer.GetSize();
					m_Size = m_End;
					bytesPerMs = m_TrackPlayer.GetByteRate() / 1000;
					m_StartMs = 0;
					m_EndMs = (int)((float)m_End / (float)bytesPerMs);
				}
				ImGui::SameLine();
			}

			// Save button
			{
				if (ImGui::Button("Save"))
				{
					BackBeat::SampleBuilder::SaveSample(m_TrackPlayer.GetTrack(), m_Start, m_End);
					ImGui::CloseCurrentPopup();
				}
				ImGui::SameLine();
			}

			// Clear button
			{
				if (ImGui::Button("Clear"))
				{
					m_TrackPlayer.ClearTrack();
					strcpy_s(trackName, "");
					m_Zero = 0;
					m_Start = 0;
					m_End = 0;
					m_Size = 0;
					bytesPerMs = 0;
					m_StartMs = 0;
					m_EndMs = 0;
					trackSet = false;
				}
			}

			// Track Name
			ImGui::Text("Track: "); ImGui::SameLine(); ImGui::Text(trackName);

			// Renders Track Editor
			{
				ImGui::PushItemWidth(m_Width - 200.0f);
				byteRate = (float)m_TrackPlayer.GetByteRate();
				BackBeat::TimeMinSec zeroTime = BackBeat::TimeMinSec();
				BackBeat::TimeMinSec sizeTime = BackBeat::TimeMinSec();;
				if (m_TrackPlayer.IsLoaded())
				{
					zeroTime = BackBeat::Audio::GetTime((float)m_Zero / byteRate);
					sizeTime = BackBeat::Audio::GetTime((float)m_Size / byteRate);
				}
				else
				{
					zeroTime.minutes = 0;
					zeroTime.seconds = 0;
					sizeTime.minutes = 0;
					sizeTime.seconds = 0;
				}
				ImGui::Text("%d:%02d", zeroTime.minutes, zeroTime.seconds); ImGui::SameLine();
				if (BackBeat::ImGuiWidgets::ImGuiTrackEditor("##", &m_Start, &m_End, &m_Zero, &m_Size, "", ImGuiSliderFlags(0)))
				{
					m_TrackPlayer.Pause();
				}
				ImGui::SameLine(); ImGui::Text("%d:%02d", sizeTime.minutes, sizeTime.seconds);
				ImGui::Spacing();
			}

			// Renders volume seekbar
			{
				ImGui::PushItemWidth(m_Width - 200.0f);
				ImGui::Text("    "); ImGui::SameLine();
				BackBeat::ImGuiWidgets::ImGuiSeekBarFloat("Volume", &m_Volume, 1.0f, "", ImGuiSliderFlags(0));
				m_TrackPlayer.SetVolume(m_Volume);
				ImGui::Spacing();
			}

			// Renders Editor Buttons
			{
				if (!m_TrackPlayer.IsPlaying())
				{
					if (ImGui::Button("Play"))
					{
						m_TrackPlayer.SetPosition(m_Start);
						m_TrackPlayer.On();
						m_TrackPlayer.Start();
					}
				}
				else if (m_TrackPlayer.IsLoaded())
				{
					if (ImGui::Button("Stop"))
						m_TrackPlayer.Pause();
				}
				ImGui::SameLine();

				// Loop
				ImGui::Checkbox("Loop", &m_Looping);
				ImGui::SameLine();

				// Zoom In
				int padding = bytesPerMs;
				if (ImGui::Button("Zoom in"))
				{
					if (trackSet && (m_End - m_Start >= padding))
					{
						m_Zero = m_Start;
						m_Size = m_End;
					}
				}
				ImGui::SameLine();

				// Zoom Out
				if (ImGui::Button("Zoom Out"))
				{
					if (trackSet)
					{
						int zoomIncrement = (int)byteRate;
						int trackSize = (int)m_TrackPlayer.GetSize();
						m_Zero = zoomIncrement > m_Zero
							? 0 : (m_Zero - zoomIncrement);
						m_Size = (m_Size + zoomIncrement) > trackSize
							? trackSize : (m_Size + zoomIncrement);
					}
				}
				ImGui::SameLine();

				// Zoom Reset
				if (ImGui::Button("Reset Zoom"))
				{
					if (trackSet)
					{
						m_Zero = 0;
						m_Size = m_TrackPlayer.GetSize();
					}
				}
			}

			static ImGuiTableFlags table_flags = 0;
			ImGui::BeginTable("SampleEditor", 2, table_flags, ImVec2(0.0f, 0.0f), 0.0f);
			const unsigned int incrementMs = 10;
			// Renders Start Input
			{
				ImGui::TableNextColumn();

				float byteRate = (float)m_TrackPlayer.GetByteRate();
				BackBeat::TimeMinSec startTime = BackBeat::TimeMinSec();
				if (m_TrackPlayer.IsLoaded())
				{
					startTime = BackBeat::Audio::GetTime((float)m_Start / byteRate);
				}
				else
				{
					startTime.minutes = 0;
					startTime.seconds = 0;
				}

				ImGui::Text("Start: %d:%02d", startTime.minutes, startTime.seconds); ImGui::SameLine();
				if (trackSet)
				{
					m_StartMs = (int)((float)m_Start / (float)bytesPerMs);
					if (ImGui::InputInt("(in ms)##StartMs", &m_StartMs, incrementMs, incrementMs * 10, ImGuiInputTextFlags(0)))
					{
						m_Start = m_StartMs < (m_EndMs - (int)incrementMs) ? (m_StartMs * bytesPerMs) : (m_EndMs - (int)incrementMs);
					}
				}
				else
				{
					int dummyStartZero = 0;
					ImGui::InputInt("(in ms)", &dummyStartZero);
					dummyStartZero = 0;
				}
			}

			// Renders End Input
			{
				ImGui::TableNextColumn();

				BackBeat::TimeMinSec endTime = BackBeat::TimeMinSec();
				if (m_TrackPlayer.IsLoaded())
				{
					endTime = BackBeat::Audio::GetTime((float)m_End / byteRate);
				}
				else
				{
					endTime.minutes = 0;
					endTime.seconds = 0;
				}

				ImGui::Text("End: %d:%02d", endTime.minutes, endTime.seconds); ImGui::SameLine();
				if (trackSet)
				{
					m_EndMs = (int)((float)m_End / (float)bytesPerMs);
					if (ImGui::InputInt("(in ms)##EndMs", &m_EndMs, incrementMs, incrementMs * 10, ImGuiInputTextFlags(0)))
					{
						m_End = m_EndMs > (m_StartMs + (int)incrementMs) ? (m_EndMs * bytesPerMs) : (m_StartMs + (int)incrementMs);
					}
				}
				else
				{
					int dummyEndZero = 0;
					ImGui::InputInt("(in ms)", &dummyEndZero);
					dummyEndZero = 0;
				}
			}

			// Total Time
			{
				ImGui::TableNextColumn();

				BackBeat::TimeMinSec totalTime = BackBeat::TimeMinSec();
				if (m_TrackPlayer.IsLoaded())
				{
					float byteRate = (float)m_TrackPlayer.GetByteRate();
					totalTime = BackBeat::Audio::GetTime((float)(m_End - m_Start) / byteRate);
					totalTime.milliseconds = m_EndMs - m_StartMs;
				}
				else
				{
					totalTime.minutes = 0;
					totalTime.seconds = 0;
					totalTime.milliseconds = 0;
				}
				ImGui::Text("Total: %d:%02d (%d ms)", totalTime.minutes, totalTime.seconds, totalTime.milliseconds);

			}
			ImGui::EndTable();

			// Set Start and End values
			if (m_TrackPlayer.IsLoaded())
			{
				m_TrackPlayer.SetStart(m_Start);
				m_TrackPlayer.SetEnd(m_End);
				if ((int)m_TrackPlayer.GetPosition() >= m_End)
				{
					if (m_Looping)
						m_TrackPlayer.SetPosition(m_Start);
					else
						m_TrackPlayer.Pause();
				}
			}

			ImGui::PopID();
			ImGui::End();
		}

		if (!m_Open)
		{
			m_TrackPlayer.Stop();
			m_TrackPlayer.ClearTrack();
			strcpy_s(trackName, "");
			m_Zero = 0;
			m_Start = 0;
			m_End = 0;
			m_Size = 0;
			bytesPerMs = 0;
			m_StartMs = 0;
			m_EndMs = 0;
			m_Volume = 1.0f;
		}
	}

	void SampleSplicer::Add(BackBeat::Mixer* mixer)
	{
		mixer->PushProcessor(m_TrackPlayer.GetProc());
	}

	void SampleSplicer::Delete(BackBeat::Mixer* mixer)
	{
		mixer->DeleteProcessor(m_TrackPlayer.GetID());
	}

	void SampleSplicer::Close()
	{
		m_Open = false;
		m_TrackPlayer.Stop();
		m_TrackPlayer.ClearTrack();
		m_Zero = 0;
		m_Start = 0;
		m_End = 0;
		m_Size = 0;
		m_StartMs = 0;
		m_EndMs = 0;
		m_Volume = 1.0f;
	}

}