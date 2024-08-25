#include "bbpch.h"

#include "ImGuiWidgets.h"

namespace BackBeat {

	bool ImGuiWidgets::ImGuiTimeline()
	{
		ImGuiWindow* window = ImGui::GetCurrentWindow();
		if (window->SkipItems)
			return false;



		ImVec2 pos = window->DC.CursorPos;
		ImRect background = window->InnerClipRect; 

		return true;
	}

}