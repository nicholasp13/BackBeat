#include "SampleSplicer.h"
namespace Exampler {

	static const float s_KnobSpeed = 0.0f;
	static const char* s_KnobFormatFloat = "%.2f";
	static const char* s_KnobFormatFloatFreq = "%.0f";
	static const char* s_KnobFormatInt = "%i";

	SampleSplicer::SampleSplicer()
	{
		
	}

	SampleSplicer::~SampleSplicer()
	{
		m_TrackPlayer.Stop();
		m_Splicer.Clear();
	}

	// Since Splicer functions take a noticeable amount of time, the user updates BackBeat:Splicer by pressing a button
	void SampleSplicer::Update()
	{
		// Set Start and End values and loop if needed
		if (m_TrackPlayer.IsLoaded())
		{
			m_TrackPlayer.SetStart(m_Start);
			m_TrackPlayer.SetEnd(m_End);
			if ((int)m_TrackPlayer.GetPosition() >= m_End)
			{
				if (m_LoopingTrack)
					m_TrackPlayer.SetPosition(m_Start);
				else
					m_TrackPlayer.Pause();
			}
		}

		// Loop sample
		auto splicerPlayer = m_Splicer.GetPlayer();
		if (splicerPlayer->IsLoaded())
		{
			if (m_LoopingSample)
				splicerPlayer->LoopOn();
			else
				splicerPlayer->LoopOff();
		}

		// Changes m_TimePercent from -100% : 100% to TimeStretcher's 0.5 : 2.0 
		if (m_TimePercent == 0)
			m_Params.timeStretcherParams.ratio = 1.0f;
		else if (m_TimePercent < 0)
			m_Params.timeStretcherParams.ratio = 1.0f + (float(m_TimePercent) / 100.0f * 0.5f);
		else
			m_Params.timeStretcherParams.ratio = 1.0f + (float(m_TimePercent) / 100.0f * 1.0f);
	}

	void SampleSplicer::Render()
	{
		if (!m_Open)
			return;

		unsigned int count = SetColors();

		const ImGuiViewport* mainViewport = ImGui::GetMainViewport();
		float x = mainViewport->WorkPos.x;
		float y = mainViewport->WorkPos.y;
		ImGui::SetNextWindowPos(ImVec2(x, y), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize(ImVec2(m_Width, m_Height), ImGuiCond_Once);
		// Sampler flags
		ImGuiWindowFlags splicerWindowFlags = 0;
		splicerWindowFlags |= ImGuiWindowFlags_MenuBar;
		splicerWindowFlags |= ImGuiWindowFlags_NoCollapse;
		splicerWindowFlags |= ImGuiWindowFlags_NoResize;

		ImGui::Begin("Sample Splicer", &m_Open, splicerWindowFlags);

		RenderMenuBar();

		if (m_RenderTrack)
			RenderTrackControls();
		else
			RenderSplicerControls();

		ImGui::End();

		if (!m_Open)
			Close();

		ImGui::PopStyleColor(count);
	}

	void SampleSplicer::Add(BackBeat::Mixer* mixer)
	{
		mixer->PushProcessor(m_TrackPlayer.GetProc());
		mixer->PushProcessor(m_Splicer.GetProc());
	}

	void SampleSplicer::Delete(BackBeat::Mixer* mixer)
	{
		mixer->DeleteProcessor(m_TrackPlayer.GetID());
		mixer->DeleteProcessor(m_Splicer.GetID());
	}

	void SampleSplicer::Close()
	{
		// Track
		m_Open = false;
		m_RenderTrack = true;
		m_TrackPlayer.Stop();
		m_TrackPlayer.ClearTrack();
		m_Track = nullptr;
		m_Zero = 0;
		m_Start = 0;
		m_End = 0;
		m_Size = 0;
		m_StartMs = 0;
		m_EndMs = 0;
		m_Volume = 1.0f;

		// Sample
		m_TimePercent = 0;
		m_Splicer.Clear();
		m_Params.Reset();
	}

	void SampleSplicer::RenderMenuBar()
	{
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::MenuItem("Track"))
			{
				m_RenderTrack = true;
			}

			if (ImGui::MenuItem("Sample"))
			{
				m_RenderTrack = false;
			}

			ImGui::EndMenuBar();
		}
	}

	void SampleSplicer::RenderTrackControls()
	{
		ImGui::PushID(m_TrackPlayer.GetID().ToString().c_str());

		RenderTrack();

		RenderTrackButtons();

		RenderTrackStartEndControls();

		ImGui::PopID();
	}

	void SampleSplicer::RenderSplicerControls()
	{
		ImGui::PushID(m_Splicer.GetID().ToString().c_str());

		RenderSplicerBuffers();
		RenderButtons();

		// Table dimensions
		const int numColumns = 5;
		const int numKnobs = 7;
		const int numGainKnobs = 3;
		const float padding = 5.0f;
		const float tableLength = m_Width - padding * 2.0f;
		const float firstColumnLength = tableLength / (float)numKnobs * (float)numGainKnobs - 150.0f;

		// Table flags
		ImGuiTableFlags tableFlags = 0;
		tableFlags |= ImGuiTableFlags_RowBg;
		tableFlags |= ImGuiTableFlags_Borders;

		// Set position of table
		ImVec2 position = ImVec2(padding, ImGui::GetCursorPos().y);
		ImGui::SetCursorPos(position);

		ImGui::BeginTable("Splicer Knobs", numColumns, tableFlags, ImVec2(tableLength, 0.0f), 0.0f);
		ImGui::TableSetupColumn("one", ImGuiTableColumnFlags_WidthFixed, firstColumnLength);

		RenderGainControls();

		RenderPitchControls();

		RenderTimeControls();

		RenderFilterControls();

		ImGui::EndTable();

		ImGui::PopID();
	}

	void SampleSplicer::RenderTrack()
	{
		// Renders Track info
		const unsigned int charLimit = 80;
		char trackName[charLimit] = {};
		bool trackSet = m_TrackPlayer.IsLoaded();
		int bytesPerMs = 0;
		float byteRate = 0.0f;

		if (trackSet)
		{
			bytesPerMs = m_TrackPlayer.GetByteRate() / 1000;
			strcpy_s(trackName, m_TrackPlayer.GetTrackName().c_str());
		}

		// Open button
		{
			if (ImGui::Button("Open"))
			{
				m_TrackPlayer.Pause();
				auto filePath = BackBeat::FileDialog::OpenFile("WAV Files (*.wav)\0*.wav\0");
				auto trackToCopy = BackBeat::TrackFactory::BuildMappedTrack(filePath);

				if (trackToCopy)
				{
					m_Track = nullptr;
					m_TrackPlayer.ClearTrack();

					// Copy/translate data to a floating point track
					m_Props = trackToCopy->GetProps();

					if (m_Props.bitDepth != BackBeat::Audio::FloatBitSize)
					{
						m_Props.bitDepth = BackBeat::Audio::FloatBitSize;
						m_Props.blockAlign = m_Props.numChannels * BackBeat::Audio::FloatByteSize;
						m_Props.format = BackBeat::Audio::FormatFloatingPoint;
						m_Props.byteRate = m_Props.sampleRate * m_Props.blockAlign;
					}

					m_Track = BackBeat::TrackFactory::BuildMappedTempTrack(m_TrackPlayer.GetID(), m_Props);

					if (m_Track)
					{
						BackBeat::TrackFactory::CopyTrackData(trackToCopy, m_Track);

						m_Track->SetName(trackToCopy->GetName());
						m_TrackPlayer.LoadTrack(m_Track);
						strcpy_s(trackName, m_TrackPlayer.GetTrackName().c_str());
						trackSet = m_TrackPlayer.IsLoaded();
						m_Zero = 0;
						m_Start = 0;
						m_End = m_Track->GetSize();
						m_Size = m_End;
						bytesPerMs = m_TrackPlayer.GetByteRate() / 1000;
						m_StartMs = 0;
						m_EndMs = (int)((float)m_End / (float)bytesPerMs);
					}
					else
						BB_CLIENT_ERROR("Error loading track to SampleSplicer");
				}
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

		// Renders Track plot
		const float visualMax = 1.0f;
		const float padding = 5.0f;
		const float width = m_Width - padding * 2.0f;
		const float height = 175.0f;
		unsigned int numBytes = BufferSize * BackBeat::Audio::FloatByteSize;
		unsigned int numSamples = BufferSize;
		unsigned int length = (m_End - m_Start);
		float progress = 0.0f;

		BackBeat::TimeMinSec sizeTime = BackBeat::TimeMinSec();
		unsigned int sizeTimeMs = m_EndMs - m_StartMs;

		BackBeat::Audio::FlushBuffer((byte*)m_Buffer.get(), numBytes);

		if (m_Track)
		{
			auto props = m_Track->GetProps();
			unsigned int position = m_Start - (m_Start % props.blockAlign);

			// Calculate the size of the data displayed
			if (numBytes > length / props.numChannels)
			{
				numBytes = length / props.numChannels;
				numBytes -= (numBytes % props.blockAlign);
				numSamples = numBytes / props.blockAlign * props.numChannels;

				sizeTime = BackBeat::Audio::GetTime((float)numBytes / props.byteRate * props.numChannels);

				// Calculate progress
				if (m_TrackPlayer.IsPlaying())
				{
					unsigned int progressPos = m_Track->GetPosition();
					progress = ((float)progressPos - (float)m_Start) / (float)numBytes / props.numChannels;
				}
				else
					progress = 0.0f;

			}
			else
			{
				numBytes -= (numBytes % props.blockAlign);
				numSamples = numBytes / props.blockAlign * props.numChannels;

				sizeTime = BackBeat::Audio::GetTime((float)numBytes / props.byteRate * props.numChannels);
				sizeTimeMs = 10000;

				// Calculate progress
				if (m_TrackPlayer.IsPlaying())
				{
					unsigned int progressPos = m_Track->GetPosition();
					progress = ((float)progressPos - (float)m_Start) / (float)numBytes / m_Props.numChannels;
				}
				else
					progress = 0.0f;
			}

			if (!m_Loader.Load(m_Track, (byte*)m_Buffer.get(), numBytes, position, 0))
				BB_CLIENT_INFO("FAILED TO LOAD");

			auto pos = ImGui::GetCursorPos();
			pos.x = padding;
			ImGui::SetCursorPos(pos);

			BackBeat::ImGuiWidgets::ImGuiTimeline("", m_Buffer.get(), numSamples, 1, "",
				visualMax * -1, visualMax, ImVec2(width, height),
				BackBeat::Audio::FloatByteSize, progress);

			ImGui::Text("Size: %d:%02d", sizeTime.minutes, sizeTime.seconds); ImGui::SameLine();
			ImGui::Text("(%d ms)", sizeTimeMs);
		}
		else
		{
			// Creates empty timeline
			auto pos = ImGui::GetCursorPos();
			pos.x = padding;
			ImGui::SetCursorPos(pos);

			BackBeat::ImGuiWidgets::ImGuiTimeline("", m_Buffer.get(), BufferSize, 1, "",
				visualMax * -1, visualMax, ImVec2(width, height),
				BackBeat::Audio::FloatByteSize, progress);

			ImGui::Text("Size: %d:%02d", 0, 0); ImGui::SameLine();
			ImGui::Text("(%d ms)", 0);
		}

		// Renders Track Editor
		{
			ImGui::PushItemWidth(m_Width - 200.0f);
			float byteRate = (float)m_TrackPlayer.GetByteRate();
			BackBeat::TimeMinSec zeroTime = BackBeat::TimeMinSec();
			BackBeat::TimeMinSec sizeTime = BackBeat::TimeMinSec();
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
		}

		ImGui::Dummy(ImVec2(0.0f, m_DummyHeight));
	}

	// Track button controls
	void SampleSplicer::RenderTrackButtons()
	{
		bool trackSet = m_TrackPlayer.IsLoaded();
		int bytesPerMs = 0;
		float byteRate = 0.0f;

		if (trackSet)
		{
			bytesPerMs = m_TrackPlayer.GetByteRate() / 1000;
			byteRate = (float)m_TrackPlayer.GetByteRate();
		}

		// Table dimensions
		const int numColumns = 1;
		const float padding = 5.0f;
		const float tableLength = m_Width - padding * 2.0f;

		// Table flags
		ImGuiTableFlags tableFlags = 0;
		tableFlags |= ImGuiTableFlags_RowBg;
		tableFlags |= ImGuiTableFlags_Borders;

		// Set Table position
		ImVec2 position = ImVec2(padding, ImGui::GetCursorPos().y);
		ImGui::SetCursorPos(position);
		ImGui::BeginTable("Row1", numColumns, tableFlags, ImVec2(tableLength, 0.0f), 0.0f);

		ImGui::TableNextColumn();
		ImGui::Dummy(ImVec2(0.0f, m_DummyHeight));

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
			else if (trackSet)
			{
				if (ImGui::Button("Stop"))
					m_TrackPlayer.Pause();
			}
			ImGui::SameLine();

			// Loop
			ImGui::Checkbox("Loop", &m_LoopingTrack);
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

			// Load splicer
			{
				bool disabled = true;
				if (m_TrackPlayer.IsLoaded())
				{
					int tenSeconds = m_TrackPlayer.GetByteRate() * 10;
					disabled = (tenSeconds < m_End - m_Start);
				}

				ImGui::BeginDisabled(disabled);

				ImGui::SameLine();
				const float loadButtonXOffset = 435.0f;
				auto pos = ImGui::GetCursorPos();
				pos.x += loadButtonXOffset;
				ImGui::SetCursorPos(pos);

				if (ImGui::Button("Load sample"))
				{
					m_Splicer.SetSampleData(m_Track, m_Start, m_End);
					m_Params.Reset();
					m_TimePercent = 0;
					m_RenderTrack = false;
				}

				ImGui::EndDisabled();

				ImGui::SameLine();
				BackBeat::ImGuiWidgets::HelpMarker("Must be less than 10 seconds to load into splicer");
			}
		}

		ImGui::Dummy(ImVec2(0.0f, m_DummyHeight));

		ImGui::EndTable();
	}

	// Start and end fine controls
	void SampleSplicer::RenderTrackStartEndControls()
	{
		const unsigned int incrementMs = 10;
		const unsigned int zeroCrossingSeconds = 1;
		const float markButtonXOffset = 87.0f;

		bool trackSet = m_TrackPlayer.IsLoaded();
		int bytesPerMs = 0;
		float byteRate = 0.0f;

		if (trackSet)
		{
			bytesPerMs = m_TrackPlayer.GetByteRate() / 1000;
			byteRate = (float)m_TrackPlayer.GetByteRate();
		}

		// Table dimensions
		const int numColumns = 2;
		const float padding = 5.0f;
		const float tableLength = m_Width - padding * 2.0f;

		// Table flags
		ImGuiTableFlags tableFlags = 0;
		tableFlags |= ImGuiTableFlags_RowBg;
		tableFlags |= ImGuiTableFlags_Borders;

		// Set Table position
		ImVec2 position = ImVec2(padding, ImGui::GetCursorPos().y);
		ImGui::SetCursorPos(position);
		ImGui::BeginTable("Row2", numColumns, tableFlags, ImVec2(tableLength, 0.0f), 0.0f);

		// Renders Start Input
		{
			ImGui::TableNextColumn();
			
			ImGui::SeparatorText("Start");

			float byteRate = (float)m_TrackPlayer.GetByteRate();
			BackBeat::TimeMinSec startTime = BackBeat::TimeMinSec();

			if (trackSet)
			{
				startTime = BackBeat::Audio::GetTime((float)m_Start / byteRate);
			}
			else
			{
				startTime.minutes = 0;
				startTime.seconds = 0;
			}

			ImGui::Text("%d:%02d", startTime.minutes, startTime.seconds); ImGui::SameLine();
			if (trackSet)
			{
				m_StartMs = (int)((float)m_Start / (float)bytesPerMs);

				if (ImGui::InputInt("(in ms)##StartMs", &m_StartMs, incrementMs, incrementMs * 10, ImGuiInputTextFlags(0)))
				{
					m_Start = m_StartMs < (m_EndMs - (int)incrementMs) ? (m_StartMs * bytesPerMs) : (m_EndMs - (int)incrementMs);
				}

				// Zero crossing finder buttons
				ImGui::Text("Find zero crossing:");
				ImGui::SameLine();

				if (ImGui::ArrowButton("Backwards_Start", ImGuiDir_Left))
				{
					m_Start = BackBeat::ZeroCrossingFinder::FindZeroCrossing(m_Track, m_Start, false, 1);
				}

				ImGui::SameLine();

				if (ImGui::ArrowButton("Forwards_Start", ImGuiDir_Right))
				{
					m_Start = BackBeat::ZeroCrossingFinder::FindZeroCrossing(m_Track, m_Start, true, 1);
				}

				ImGui::SameLine();
				auto pos = ImGui::GetCursorPos();
				pos.x += markButtonXOffset;
				ImGui::SetCursorPos(pos);

				if (ImGui::Button("Mark##Start") && m_TrackPlayer.IsPlaying())
				{
					m_Start = m_TrackPlayer.GetPosition();
				}
			}
			else
			{
				int dummyStartZero = 0;
				ImGui::InputInt("(in ms)", &dummyStartZero);
				dummyStartZero = 0;

				ImGui::Text("Find zero crossing:");
				ImGui::SameLine();
				ImGui::ArrowButton("", ImGuiDir_Left);
				ImGui::SameLine();
				ImGui::ArrowButton("", ImGuiDir_Right);

				ImGui::SameLine();
				auto pos = ImGui::GetCursorPos();
				pos.x += markButtonXOffset;
				ImGui::SetCursorPos(pos);

				ImGui::Button("Mark##Start_Dummy");
			}

			ImGui::Dummy(ImVec2(0.0f, m_DummyHeight));
		}

		// Renders End Input
		{
			ImGui::TableNextColumn();
			
			ImGui::SeparatorText("End");

			BackBeat::TimeMinSec endTime = BackBeat::TimeMinSec();

			if (trackSet)
			{
				endTime = BackBeat::Audio::GetTime((float)m_End / byteRate);
			}
			else
			{
				endTime.minutes = 0;
				endTime.seconds = 0;
			}

			ImGui::Text("%d:%02d", endTime.minutes, endTime.seconds); ImGui::SameLine();

			if (trackSet)
			{
				m_EndMs = (int)((float)m_End / (float)bytesPerMs);
				if (ImGui::InputInt("(in ms)##EndMs", &m_EndMs, incrementMs, incrementMs * 10, ImGuiInputTextFlags(0)))
				{
					m_End = m_EndMs > (m_StartMs + (int)incrementMs) ? (m_EndMs * bytesPerMs) : (m_StartMs + (int)incrementMs);
				}

				// Zero crossing finder buttons
				ImGui::Text("Find zero crossing:");
				ImGui::SameLine();

				if (ImGui::ArrowButton("Backwards_End", ImGuiDir_Left))
				{
					m_End = BackBeat::ZeroCrossingFinder::FindZeroCrossing(m_Track, m_End, false, 1);
				}

				ImGui::SameLine();

				if (ImGui::ArrowButton("Forwards_End", ImGuiDir_Right))
				{
					m_End = BackBeat::ZeroCrossingFinder::FindZeroCrossing(m_Track, m_End, true, 1);
				}

				ImGui::SameLine();
				auto pos = ImGui::GetCursorPos();
				pos.x += markButtonXOffset + 1.0f; // For some reason this is a pixel different
				ImGui::SetCursorPos(pos);

				if (ImGui::Button("Mark##End") && m_TrackPlayer.IsPlaying())
				{
					m_End = m_TrackPlayer.GetPosition();
				}
			}
			else
			{
				int dummyEndZero = 0;
				ImGui::InputInt("(in ms)", &dummyEndZero);
				dummyEndZero = 0;

				ImGui::Text("Find zero crossing:");
				ImGui::SameLine();
				ImGui::ArrowButton("", ImGuiDir_Left);
				ImGui::SameLine();
				ImGui::ArrowButton("", ImGuiDir_Right);

				ImGui::SameLine();
				auto pos = ImGui::GetCursorPos();
				pos.x += markButtonXOffset + 1.0f; // For some reason this is a pixel different
				ImGui::SetCursorPos(pos);

				ImGui::Button("Mark##End_Dummy");
			}

			ImGui::Dummy(ImVec2(0.0f, m_DummyHeight));
		}

		ImGui::EndTable();
	}

	void SampleSplicer::RenderSplicerBuffers()
	{
		const float visualMax = 1.0f;
		const float padding = 5.0f;
		const float width = m_Width - padding * 2.0f;
		const float height = 100.0f;

		float* leftBuffer = m_Splicer.GetLeftChannel();
		float* rightBuffer = m_Splicer.GetRightChannel();

		unsigned int size = m_Splicer.GetOutputSize();
		// Size is doubled in percent calulcation because BackBeat::SplicerPlayer combines both 
		// channels into a single output
		float percent = float(m_Splicer.GetPlayer()->GetPosition()) / (float(size) * 2.0f);

		// Set position of left channel timeline
		ImVec2 leftPosition = ImVec2(padding, ImGui::GetCursorPos().y);
		ImGui::SetCursorPos(leftPosition);

		BackBeat::ImGuiWidgets::ImGuiTimeline("##LeftChannel", leftBuffer, size, 1, "Left Channel",
			visualMax * -1, visualMax, ImVec2(width, height),
			BackBeat::Audio::FloatByteSize, percent);

		// Set position of right channel timeline
		ImVec2 rightPosition = ImVec2(padding, ImGui::GetCursorPos().y);
		ImGui::SetCursorPos(rightPosition);

		BackBeat::ImGuiWidgets::ImGuiTimeline("##RightChannel", rightBuffer, size, 1, "Right Channel",
			visualMax * -1, visualMax, ImVec2(width, height),
			BackBeat::Audio::FloatByteSize, percent);

		unsigned int seconds = size / 48000;
		ImGui::Text("Size: %d Seconds", seconds);
	}

	void SampleSplicer::RenderButtons()
	{
		const float width = 60.0f;
		const float height = 20.0f;
		const ImVec2 buttonSize = ImVec2(width, height);

		auto player = m_Splicer.GetPlayer();
		bool loaded = player->IsLoaded();

		// Table dimensions
		const int numColumns = 1;
		const float padding = 5.0f;
		const float tableLength = m_Width - padding * 2.0f;

		// Table flags
		ImGuiTableFlags tableFlags = 0;
		tableFlags |= ImGuiTableFlags_RowBg;
		tableFlags |= ImGuiTableFlags_Borders;

		// Set Table position
		ImVec2 position = ImVec2(padding, ImGui::GetCursorPos().y);
		ImGui::SetCursorPos(position);
		ImGui::BeginTable("Row1", numColumns, tableFlags, ImVec2(tableLength, 0.0f), 0.0f);

		ImGui::TableNextColumn();
		ImGui::Dummy(ImVec2(0.0f, m_DummyHeight));

		ImGui::BeginDisabled(!loaded);

		// Play Sample button
		if (!player->IsOn())
		{
			if (ImGui::Button("Play", buttonSize))
				player->Play();
		}
		else
		{
			if (ImGui::Button("Pause", buttonSize))
				player->Stop();
		}

		ImGui::SameLine();

		ImGui::Checkbox("Loop", &m_LoopingSample);

		ImGui::SameLine();

		// Update Sample button
		if (ImGui::Button("Update", buttonSize))
		{
			m_Splicer.Update(m_Params);
		}

		ImGui::SameLine();

		// Save sample button
		const float saveButtonXOffset = 648.0f;
		auto pos = ImGui::GetCursorPos();
		pos.x += saveButtonXOffset;
		ImGui::SetCursorPos(pos);

		if (ImGui::Button("Save"))
		{
			m_Splicer.SaveSample();
		}

		ImGui::EndDisabled();
		ImGui::Dummy(ImVec2(0.0f, m_DummyHeight));

		ImGui::EndTable();
	}

	void SampleSplicer::RenderGainControls()
	{
		ImGui::TableNextColumn();

		ImGui::SeparatorText("Gain");

		const float dummyHeight = 19.0f;
		ImGui::Dummy(ImVec2(0.0f, dummyHeight));

		const float min = BackBeat::Audio::DBMin;
		const float max = BackBeat::Audio::DBMax;

		ImGuiKnobs::Knob("Master", &(m_Params.masterGainDB), min, max,
			s_KnobSpeed, s_KnobFormatFloat, ImGuiKnobVariant_::ImGuiKnobVariant_WiperDot);

		if (ImGui::IsItemActive() && ImGui::IsMouseDoubleClicked(0))
			m_Params.masterGainDB = BackBeat::Audio::DBDefault;

		ImGui::SameLine();

		ImGuiKnobs::Knob("Left", &(m_Params.leftGainDB), min, max,
			s_KnobSpeed, s_KnobFormatFloat, ImGuiKnobVariant_::ImGuiKnobVariant_WiperDot);

		if (ImGui::IsItemActive() && ImGui::IsMouseDoubleClicked(0))
			m_Params.leftGainDB = BackBeat::Audio::DBDefault;

		ImGui::SameLine();

		ImGuiKnobs::Knob("Right", &(m_Params.rightGainDB), min, max,
			s_KnobSpeed, s_KnobFormatFloat, ImGuiKnobVariant_::ImGuiKnobVariant_WiperDot);

		if (ImGui::IsItemActive() && ImGui::IsMouseDoubleClicked(0))
			m_Params.rightGainDB = BackBeat::Audio::DBDefault;

		ImGui::Dummy(ImVec2(0.0f, m_DummyHeight));
	}

	void SampleSplicer::RenderPitchControls()
	{
		ImGui::TableNextColumn();

		ImGui::SeparatorText("Pitch");

		const float dummyHeight = 19.0f;
		ImGui::Dummy(ImVec2(0.0f, dummyHeight));

		// Pitch shifter
		ImGuiKnobs::Knob("Semitones", &m_Params.pitchShifterParams.shiftSemitones, -12.0f, 12.0f,
			s_KnobSpeed, s_KnobFormatFloat, ImGuiKnobVariant_::ImGuiKnobVariant_WiperDot);

		if (ImGui::IsItemActive() && ImGui::IsMouseDoubleClicked(0))
			m_Params.pitchShifterParams.shiftSemitones = 0.0f;

	}

	void SampleSplicer::RenderTimeControls()
	{
		ImGui::TableNextColumn();

		ImGui::SeparatorText("Time");

		BackBeat::ImGuiWidgets::HelpMarker("Negative is shrinking\nPositive is stretching");

		ImGui::SameLine();
		const float dummyHeight = 19.0f;
		ImGui::Dummy(ImVec2(0.0f, dummyHeight));

		ImGuiKnobs::KnobInt("Percent", &m_TimePercent, -100, 100,
			s_KnobSpeed, s_KnobFormatInt, ImGuiKnobVariant_::ImGuiKnobVariant_WiperDot);

		if (ImGui::IsItemActive() && ImGui::IsMouseDoubleClicked(0))
			m_TimePercent = 0;
	}

	void SampleSplicer::RenderFilterControls()
	{
		ImGui::TableNextColumn();

		// Low pass filter
		{
			ImGui::PushID("LowPass");

			ImGui::SeparatorText("Low Pass Filter");

			ImGui::Checkbox("On", &(m_Params.lowPassFilterParams.isOn));

			ImGuiKnobs::Knob("Cutoff", &(m_Params.lowPassFilterParams.cutoff), BackBeat::SynthBase::FilterCutoffMin, BackBeat::SynthBase::FilterCutoffMax,
				s_KnobSpeed, s_KnobFormatFloatFreq, ImGuiKnobVariant_::ImGuiKnobVariant_WiperDot);

			if (ImGui::IsItemActive() && ImGui::IsMouseDoubleClicked(0))
				m_Params.lowPassFilterParams.cutoff = BackBeat::SynthBase::FilterCutoffMin;

			ImGui::PopID();
		}

		ImGui::TableNextColumn();

		// High pass filter
		{
			ImGui::PushID("HighPass");

			ImGui::SeparatorText("High Pass Filter");

			ImGui::Checkbox("On", &(m_Params.highPassFilterParams.isOn));

			ImGuiKnobs::Knob("Cutoff", &(m_Params.highPassFilterParams.cutoff), BackBeat::SynthBase::FilterCutoffMin, BackBeat::SynthBase::FilterCutoffMax,
				s_KnobSpeed, s_KnobFormatFloatFreq, ImGuiKnobVariant_::ImGuiKnobVariant_WiperDot);

			if (ImGui::IsItemActive() && ImGui::IsMouseDoubleClicked(0))
				m_Params.highPassFilterParams.cutoff = BackBeat::SynthBase::FilterCutoffMin;

			ImGui::PopID();
		}
	}

	unsigned int SampleSplicer::SetColors()
	{
		unsigned int count = 0;

		// MenuBar colors
		ImGui::PushStyleColor(ImGuiCol_MenuBarBg, IM_COL32(120, 120, 120, 255)); count++;

		// Window colors
		ImGui::PushStyleColor(ImGuiCol_Border, IM_COL32(200, 200, 200, 255)); count++;
		ImGui::PushStyleColor(ImGuiCol_WindowBg, IM_COL32(20, 20, 20, 255)); count++;

		// Table colors
		ImGui::PushStyleColor(ImGuiCol_TableRowBg, IM_COL32(24, 24, 24, 255)); count++;
		ImGui::PushStyleColor(ImGuiCol_TableRowBgAlt, IM_COL32(24, 24, 24, 255)); count++;
		ImGui::PushStyleColor(ImGuiCol_TableBorderStrong, IM_COL32(91, 91, 91, 255)); count++;
		ImGui::PushStyleColor(ImGuiCol_TableBorderLight, IM_COL32(91, 91, 91, 255)); count++;

		// Plot colors
		ImGui::PushStyleColor(ImGuiCol_PlotLines, IM_COL32(230, 230, 230, 255)); count++;
		ImGui::PushStyleColor(ImGuiCol_PlotLinesHovered, IM_COL32(230, 230, 230, 255)); count++;

		// Misc colors
		ImGui::PushStyleColor(ImGuiCol_Separator, IM_COL32(200, 200, 200, 255)); count++;
		ImGui::PushStyleColor(ImGuiCol_SliderGrab, IM_COL32(0, 90, 0, 255)); count++;
		ImGui::PushStyleColor(ImGuiCol_SliderGrabActive, IM_COL32(0, 224, 0, 255)); count++;
		ImGui::PushStyleColor(ImGuiCol_CheckMark, IM_COL32(0, 224, 0, 255)); count++;

		// Knobs colors
		// Filled
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, IM_COL32(255, 255, 255, 255)); count++;
		// Filled (Hovered)
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, IM_COL32(0, 200, 0, 255)); count++;
		// Track
		ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(180, 180, 180, 255)); count++;

		return count;
	}

}