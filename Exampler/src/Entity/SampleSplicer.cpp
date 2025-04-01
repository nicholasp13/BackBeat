// TODO: Redesign GUI
//       - Add way to decrease visual max
//       - Add zero crossing finding tool
//       - Seperate wave data shower and track player into two separate sections of splicer
//             - Top part shows the 10 secs buffer of where m_start is
//             - User can LOCK the sample into place where the player is
//             - User can play the sample seperately
//                 - The sample should be played perhaps by a new SampleSplicer BB object that allows for modulation
//                   including panning, filtering, etc. 
//             

#include "SampleSplicer.h"
namespace Exampler {

	static const float s_KnobSpeed = 0.0f;
	static const char* s_KnobFormatFloat = "%.2f";
	static const char* s_KnobFormatFloatFreq = "%.0f";
	static const char* s_KnobFormatInt = "%i";

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
		m_Volume(1.0f),
		m_TimeRatio(0.0f),
		m_Buffer(std::make_shared<float[]>(BufferSize)),
		m_Params(BackBeat::SplicerParameters()),
		m_Loader(BufferSize * BackBeat::Audio::FloatByteSize* BackBeat::Audio::Stereo),
		m_Props(BackBeat::AudioProps()),
		m_Track(nullptr)
	{
		
	}

	SampleSplicer::~SampleSplicer()
	{
		m_TrackPlayer.Stop();
	}

	// Since Splicer functions take a noticeable amount of time, the user updates splicer by pressing a button
	void SampleSplicer::Update()
	{
		
	}

	void SampleSplicer::Render()
	{
		if (!m_Open)
			return;

		const ImGuiViewport* mainViewport = ImGui::GetMainViewport();
		float x = mainViewport->WorkPos.x;
		float y = mainViewport->WorkPos.y;
		ImGui::SetNextWindowPos(ImVec2(x, y), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize(ImVec2(m_Width, m_Height), ImGuiCond_Once);
		// Sampler flags
		ImGuiWindowFlags splicerWindowFlags = 0;
		splicerWindowFlags |= ImGuiWindowFlags_NoCollapse;
		splicerWindowFlags |= ImGuiWindowFlags_NoResize;

		ImGui::Begin("Sample Splicer", &m_Open, splicerWindowFlags);

		RenderTrackControls();

		ImGui::Separator();

		RenderSplicerControls();

		ImGui::End();

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
		m_Open = false;
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
	}

	void SampleSplicer::RenderTrackControls()
	{
		ImGui::PushID(m_TrackPlayer.GetID().ToString().c_str());

		const unsigned int charLimit = 80;
		char trackName[charLimit] = {};
		std::string sampleName = "";
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

		// Render actual buffer data
		RenderBuffer();

		// Renders Track Editor
		{
			ImGui::PushItemWidth(m_Width - 200.0f);
			byteRate = (float)m_TrackPlayer.GetByteRate();
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

		if (!m_Open)
		{
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
		}

		ImGui::PopID();
	}

	void SampleSplicer::RenderSplicerControls()
	{
		ImGui::PushID(m_Splicer.GetID().ToString().c_str());

		float buttonWidth = 150.0f;
		float buttonHeight = 30.0f;
		if (ImGui::Button("Load into Splicer", ImVec2(buttonWidth, buttonHeight)))
		{
			m_Splicer.SetSampleData(m_Track, m_Start, m_End);

			// Reset params for splicer controls
			m_TimeRatio = 0.0f;
			m_Params.timeStretcherParams.ratio = 1.0f;
		}

		bool loaded = m_Splicer.IsLoaded();

		ImGui::BeginDisabled(!loaded);

		// Placehold for now
		// Left Channel
		{
			const float visualMax = 1.0f;
			const float width = m_Width - 50.0f;
			const float height = 100.0f;

			float* buffer = m_Splicer.GetLeftChannel();
			unsigned int size = m_Splicer.GetOutputSize();

			unsigned int seconds = size / 48000;
			ImGui::Text("%d Seconds", seconds);

			/*BackBeat::ImGuiWidgets::ImGuiTimeline("", buffer, size, 1, "Left Channel",
				visualMax * -1, visualMax, ImVec2(width, height),
				BackBeat::Audio::FloatByteSize, 0.0f);*/

			ImGui::PlotLines("", buffer, size, 1, "Left Channel",
				visualMax * -1, visualMax, ImVec2(width, height),
				BackBeat::Audio::FloatByteSize);
		}

		auto player = m_Splicer.GetPlayer();

		// Buttons
		{
			const float width = 60.0f;
			const float height = 20.0f;
			const ImVec2 buttonSize = ImVec2(width, height);

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

			// Update Sample button
			ImGui::SameLine();

			if (ImGui::Button("Update", buttonSize))
			{
				m_Splicer.Update(m_Params);
			}

		}

		// Knobs
		{
			ImGuiKnobs::Knob("Time", &m_TimeRatio, -1.0f, 1.0f,
				s_KnobSpeed, s_KnobFormatFloat, ImGuiKnobVariant_::ImGuiKnobVariant_WiperDot);

			if (ImGui::IsItemActive() && ImGui::IsMouseDoubleClicked(0))
				m_TimeRatio = 0.0f;

			// Changes m_TimeRatio from -1 : 1 to TimeStretcher's 0.5 : 2.0 
			float delta = 0.001f;
			if (std::abs(m_TimeRatio) < delta)
				m_Params.timeStretcherParams.ratio = 1.0f;
			else if (m_TimeRatio < 0.0f)
				m_Params.timeStretcherParams.ratio = 1.0f + (m_TimeRatio * 0.5f);
			else
				m_Params.timeStretcherParams.ratio = 1.0f + (m_TimeRatio * 1.0f);
		}

		ImGui::EndDisabled();

		ImGui::PopID();
	}

	void SampleSplicer::RenderBuffer()
	{
		const float visualMax = 1.0f;
		const float width = m_Width - 50.0f;
		const float height = 50.0f;
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

			BackBeat::ImGuiWidgets::ImGuiTimeline("", m_Buffer.get(), numSamples, 1, "",
				visualMax * -1, visualMax, ImVec2(width, height),
				BackBeat::Audio::FloatByteSize, progress);

			ImGui::Text("Size: %d:%02d", sizeTime.minutes, sizeTime.seconds); ImGui::SameLine();
			ImGui::Text("(%d ms)", sizeTimeMs);
		}
		else
		{
			// Creates empty timeline
			BackBeat::ImGuiWidgets::ImGuiTimeline("", m_Buffer.get(), BufferSize, 1, "",
				visualMax * -1, visualMax, ImVec2(width, height),
				BackBeat::Audio::FloatByteSize, progress);

			ImGui::Text("Size: %d:%02d", 0, 0); ImGui::SameLine();
			ImGui::Text("(%d ms)", 0);
		}
	}

}