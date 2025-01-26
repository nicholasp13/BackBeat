#include "Canvas.h"

// TODO: Allow for timeline to track Recording the same as Playback

namespace Exampler {

	Canvas::Canvas()
		:
		m_Position(0u),
		m_Width(0.0f), 
		m_Height(0.0f),
		m_Buffer(std::make_shared<float[]>(s_BufferSize)), 
		m_Props(BackBeat::AudioProps()),
		m_Loader(s_BufferSize* BackBeat::Audio::Stereo * sizeof(float)),
		m_PlayerMgr(nullptr),
		m_RecorderMgr(nullptr)
	{

	}

	Canvas::~Canvas()
	{

	}

	void Canvas::Init(BackBeat::AudioProps props, BackBeat::PlayerManager* playerMgr, BackBeat::RecorderManager* recorderMgr)
	{
		m_Props = props;
		m_PlayerMgr = playerMgr;
		m_RecorderMgr = recorderMgr;
	}

	void Canvas::Render(float width, float height, bool active)
	{
		if (!m_PlayerMgr)
			return;
		if (width <= 0.0f || height <= 0.0f)
			return;

		m_Width = width;
		m_Height = height;

		ImGuiWindowFlags childFlags = 0;
		childFlags |= ImGuiWindowFlags_NoTitleBar;
		childFlags |= ImGuiWindowFlags_NoMove;
		childFlags |= ImGuiWindowFlags_NoCollapse;
		childFlags |= ImGuiWindowFlags_NoResize;
		childFlags |= ImGuiWindowFlags_AlwaysVerticalScrollbar;

		if (!active)
			childFlags |= ImGuiWindowFlags_NoInputs;

		unsigned int count = SetColors();

		ImGui::BeginChild("Canvas", ImVec2(m_Width, m_Height), false, childFlags);

		if (m_Entities.size() == 0)
			ImGui::TextDisabled("Use Tracks menu to add tracks");
		else
		{
			const float tWidth = (float)width;
			const float tHeight = 1200.0f;
			static ImGuiTableFlags tableFlags = 0;
			tableFlags |= ImGuiTableFlags_RowBg;
			tableFlags |= ImGuiTableFlags_BordersInnerH;
			tableFlags |= ImGuiTableFlags_BordersOuter;

			ImGui::BeginTable("Entities", 2, tableFlags, ImVec2(tWidth, tHeight), 1.0f);
			
			// Setup ImGui table columns
			const float firstColumnWidth = 200.0f;
			ImGui::TableSetupColumn("one", ImGuiTableColumnFlags_WidthFixed, firstColumnWidth);
			ImGui::TableSetupColumn("two", ImGuiTableColumnFlags_WidthFixed, tWidth - firstColumnWidth);

			RenderEntities();

			ImGui::EndTable();
		}
		ImGui::EndChild();

		// Renders position controller
		{
			float fileLimit = (float)m_PlayerMgr->GetFileLimit();
			float positionBytes = (float)m_Position * (float)sizeof(float) / (float)m_Props.byteRate;
			static float positionPercent = 0.0f;

			BackBeat::TimeMinSec time = BackBeat::TimeMinSec();

			if (!m_RecorderMgr->IsRecording())
				time = m_PlayerMgr->GetTime();
			else
				time = m_RecorderMgr->GetTime();

			BackBeat::TimeMinSec length = BackBeat::Audio::GetTime(fileLimit / (float)m_Props.byteRate);

			ImGui::Text("Time: %d:%02d", time.minutes, time.seconds); ImGui::SameLine();
			ImGui::Text("Total: %d:%02d", length.minutes, length.seconds); ImGui::SameLine();

			if (BackBeat::ImGuiWidgets::ImGuiSeekBarFloat("##CanvasSeekbar", &positionPercent,
				1.0f, "", ImGuiSliderFlags(0)))
			{
				unsigned int position = unsigned int(positionPercent * fileLimit);
				unsigned int remainder = position % m_Props.blockAlign;
				m_Position = (position - remainder) / (unsigned int)sizeof(float);
			}
			else
				positionPercent = ((float)m_Position * (float)sizeof(float)) / fileLimit;

			if (ImGui::IsItemDeactivated())
			{
				unsigned int position = unsigned int(positionPercent * fileLimit);
				unsigned int remainder = position % m_Props.blockAlign;
				m_Position = (position - remainder) / (unsigned int)sizeof(float);

				m_PlayerMgr->SetPosition(positionPercent * fileLimit / float(m_Props.byteRate));
			}
		}

		ImGui::PopStyleColor(count);
	}

	void Canvas::AddEntity(std::shared_ptr<Entity> entity)
	{
		if (!entity)
			return;

		for (auto itr = m_Entities.begin(); itr != m_Entities.end(); itr++)
		{
			if (entity->GetID() == (*itr)->GetID())
				return;
		}
		
		m_Entities.push_back(entity);
	}

	void Canvas::DeleteEntity(std::shared_ptr<Entity> entity)
	{
		if (!entity)
			return;

		for (auto itr = m_Entities.begin(); itr != m_Entities.end(); itr++)
		{
			if (entity->GetID() == (*itr)->GetID())
			{
				m_Entities.erase(itr);
				return;
			}
		}
	}

	void Canvas::Reset()
	{
		m_Position = 0;
	}

	void Canvas::RenderEntities()
	{
		const float height = 150.0f;
		const float firstColumnWidth = 200.0f;
		const float padding = 27.0f;
		const float secondColumnWidth = m_Width - firstColumnWidth - padding;
		float progress = 0.0f;
		float secondsPerBuffer = 0.0f;
		float secondsPlayed = 0.0f;
		float positionInSeconds = 0.0f;
		unsigned int numChannels = 0;
		unsigned int numBytes = s_BufferSize * sizeof(float);
		unsigned int pos = m_Position * sizeof(float);
		std::string label = "";
		std::string overlayText = "";
		const float visualMax = 1.00f;
		std::shared_ptr<BackBeat::MappedTrack> track = nullptr;

		// Calculate the progress tracker
		secondsPerBuffer = (float)(s_BufferSize * sizeof(float)) / (float)m_Props.byteRate * (float)m_Props.numChannels;
		positionInSeconds = (float)pos / (float)m_Props.byteRate;

		if (!m_RecorderMgr->IsRecording())
			secondsPlayed = float(m_PlayerMgr->GetTimeMs().milliseconds) / 1000.0f;
		else
			secondsPlayed = float(m_RecorderMgr->GetTimeMs().milliseconds) / 1000.0f;

		progress = (secondsPlayed - positionInSeconds) / secondsPerBuffer;

		if (progress > 1.0f)
		{
			m_Position += (s_BufferSize * 2);
			pos = m_Position * sizeof(float);
			progress = 0.0f;
		}

		// Flush buffer before each call to Load()
		for (auto itr = m_Entities.begin(); itr != m_Entities.end(); itr++)
		{
			std::shared_ptr<Entity> entity = *itr;
			auto id = entity->GetID();
			ImGui::PushID(id.ToString().c_str());

			// Renders entity controls
			{
				ImGui::TableNextColumn();
				entity->ImGuiRender();
			}

			// Renders track data
			{
				ImGui::TableNextColumn();

				track = entity->GetMappedTrack();
				BackBeat::Audio::FlushBuffer((byte*)m_Buffer.get(), numBytes);

				if (track)
				{
					numChannels = track->GetProps().numChannels;

					for (unsigned int i = 0; i < numChannels; i++)
					{
						label = track->GetName() + std::to_string(i);
						overlayText = "Channel " + std::to_string(i + 1);

						if (numChannels != m_Props.numChannels)
							m_Loader.Load(track, (byte*)m_Buffer.get(), numBytes, pos / m_Props.numChannels * numChannels, i);
						else
							m_Loader.Load(track, (byte*)m_Buffer.get(), numBytes, pos, i);

						BackBeat::ImGuiWidgets::ImGuiTimeline(label.c_str() , m_Buffer.get(), s_BufferSize, 1, overlayText.c_str(),
							visualMax * -1, visualMax, ImVec2(secondColumnWidth, height / (float)numChannels),
							BackBeat::Audio::FloatByteSize, progress);
					}
				}
				else
				{
					// Creates empty timeline
					overlayText = "Channel 1";
					BackBeat::ImGuiWidgets::ImGuiTimeline("", m_Buffer.get(), s_BufferSize, 1, overlayText.c_str(),
						visualMax * -1, visualMax, ImVec2(secondColumnWidth, height),
						BackBeat::Audio::FloatByteSize, progress);
				}

			}

			ImGui::PopID();
			track = nullptr;
		}

	}

	unsigned int Canvas::SetColors()
	{
		unsigned int count = 0;

		// Main background
		ImGui::PushStyleColor(ImGuiCol_WindowBg, IM_COL32(147, 157, 169, 255)); count++;
		ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(46, 46, 46, 255)); count++;

		// Table colors
		ImGui::PushStyleColor(ImGuiCol_TableRowBg, IM_COL32(147, 157, 169, 255)); count++;
		ImGui::PushStyleColor(ImGuiCol_TableRowBgAlt, IM_COL32(147, 157, 169, 255)); count++;
		ImGui::PushStyleColor(ImGuiCol_TableBorderStrong, IM_COL32(72, 72, 72, 255)); count++;
		ImGui::PushStyleColor(ImGuiCol_TableBorderLight, IM_COL32(72, 72, 72, 255)); count++;

		return count;
	}

}
