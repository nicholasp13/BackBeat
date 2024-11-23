#include "DummyEntity.h"
namespace Exampler {

	Dummy::Dummy()
		: m_Open(false), m_Name("Dummy")
	{

	}

	Dummy::~Dummy()
	{

	}

	void Dummy::ImGuiRender()
	{
		ImGui::PushID("Dummy");

		ImGui::Spacing();
		if (ImGui::Button("Open Dummy"))
			m_Open = true;

		if (!m_Open)
		{
			ImGui::PopID();
			return;
		}
		ImGui::Spacing();

		// Renders Actual Entity
		const ImGuiViewport* mainViewport = ImGui::GetMainViewport();
		float x = mainViewport->WorkPos.x;
		float y = mainViewport->WorkPos.y;
		ImGui::SetNextWindowPos(ImVec2(x, y), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize(ImVec2(1200.0f, 600.0f), ImGuiCond_Once);
		// Flags
		ImGuiWindowFlags dummy_window_flags = 0;
		dummy_window_flags |= ImGuiWindowFlags_NoCollapse;
		dummy_window_flags |= ImGuiWindowFlags_MenuBar;
		dummy_window_flags |= ImGuiWindowFlags_NoResize;

		ImGui::Begin("Dummy", &m_Open, dummy_window_flags);







		ImGui::End();
		ImGui::PopID();
	}

}