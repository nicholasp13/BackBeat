#include "Canvas.h"

namespace Exampler {


	Canvas::Canvas()
		: m_Width(0.0f), m_Height(0.0f), m_PlayerMgr(nullptr)
	{

	}

	Canvas::~Canvas()
	{

	}

	void Canvas::Init(BackBeat::PlayerManager* playerMgr)
	{
		m_PlayerMgr = playerMgr;
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
			tableFlags |= ImGuiTableFlags_BordersInner;
			tableFlags |= ImGuiTableFlags_NoPadOuterX;
			tableFlags |= ImGuiTableFlags_NoPadInnerX;

			ImGui::BeginTable("Entities", 2, tableFlags, ImVec2(tWidth, tHeight), 0.0f);
			
			// Setup ImGui table columns
			const float firstColumnWidth = 200.0f;
			ImGui::TableSetupColumn("one", ImGuiTableColumnFlags_WidthFixed, firstColumnWidth); // Default to 100.0f
			ImGui::TableSetupColumn("two", ImGuiTableColumnFlags_WidthFixed, tWidth - firstColumnWidth);

			RenderEntities();

			ImGui::EndTable();
		}
		ImGui::EndChild();

		// TODO: Render Popup here

		ImGui::Text("Position: %d:%02d", 0, 0); ImGui::SameLine();
		ImGui::Text("Total: %d:%02d", 0, 0);

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

	void Canvas::RenderEntities()
	{
		// Size of track render
		const float firstColumnWidth = 200.0f;
		float trackWidth = m_Width - firstColumnWidth;
		auto size = ImVec2(trackWidth, 150.0f);

		// Placeholder for track render buffer
		const float visualMax = .15f;
		const int bufferSize = 100;
		float buffer[bufferSize] = {};
		memset(buffer, 0, (size_t)bufferSize * sizeof(float));

		for (auto itr = m_Entities.begin(); itr != m_Entities.end(); itr++)
		{
			// Renders entity controls
			{
				ImGui::TableNextColumn();

				std::shared_ptr<Entity> entity = *itr;
				entity->ImGuiRender();
			}

			// Renders track
			{
				ImGui::TableNextColumn();

				// Placeholder for testing looks
				ImGui::PlotLines("", buffer, bufferSize, 1, "",
					visualMax * -1, visualMax, size);
			}
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
