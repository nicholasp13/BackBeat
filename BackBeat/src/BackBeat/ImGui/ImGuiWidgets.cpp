#include "bbpch.h"

#include <imgui.h>
#include <imgui_internal.h>
#include "ImGuiWidgets.h"
namespace BackBeat {

	bool ImGuiWidgets::ImGuiTimeline()
	{
		ImGuiWindow* window = ImGui::GetCurrentWindow();
		if (window->SkipItems)
			return false;

		ImGui::PushID("Timeline");

		ImVec2 pos = window->DC.CursorPos;
	    ImRect background = window->InnerClipRect;

		// Draw backgroumd
		// Draw Tracks:
    	// for i tracks draw
		//     Info rectangle with track name, current time, length with button for play, pause stop
		//     To the right of that create a frame around another rectangle to the right of indefinite length
		//     Inner rectangle is a solid neutral color bar that goes on for a bit
		//     Overlayed rectangle is progress bar with a lighter color and finishes at the end of the track
		//         with a solid colored line at the end to mark the end (might be another border around the track)
		// Bottom is a scroll bar to go right until the arbitrary end


		ImGui::PopID();
		return true;
	}

	// NOTE: - Future implementation might want the track to keep playing while visually the seek bar changes
	//           and only when the bar is released then the position is changed. (This does not work as either
	//           the bar does not update while the track plays but does change only when released OR the
	//           bar does not move or change with the track position)
	// COLORS:
	//     - Frame rectangle    - ImGui::GetColorU32(g.ActiveId == id ? ImGuiCol_FrameBgActive : ImGuiCol_FrameBg)
	//     - Progress rectangle - ImGuiCol_FrameBgHovered
	//     - Grab rectangle     - ImGui::GetColorU32(g.ActiveId == id ? ImGuiCol_SliderGrabActive : ImGuiCol_SliderGrab), style.GrabRounding)
	bool ImGuiWidgets::ImGuiSeekBar(const char* label, ImGuiDataType dataType, float fraction, void* pData, const void* pMin, const void* pMax,
		const char* format, ImGuiSliderFlags flags)
	{
		ImGuiWindow* window = ImGui::GetCurrentWindow();
		if (window->SkipItems)
			return false;
		
		ImGuiContext& g = *GImGui;
		const ImGuiStyle& style = g.Style;
		const ImGuiID id = window->GetID(label);
		const float w = ImGui::CalcItemWidth();
		const ImVec2 labelSize = ImGui::CalcTextSize(label, NULL, true);
		const ImRect frameRect(window->DC.CursorPos, Add(window->DC.CursorPos , ImVec2(w, labelSize.y + style.FramePadding.y * 2.0f)));
		const ImRect totalRect(frameRect.Min, Add(frameRect.Max, ImVec2(labelSize.x > 0.0f ? style.ItemInnerSpacing.x + labelSize.x : 0.0f, 0.0f)));

		ImGui::ItemSize(totalRect, style.FramePadding.y);
		if (!ImGui::ItemAdd(totalRect, id, &frameRect, 0))
			return false;

		// Default format string when passing NULL
		if (format == NULL)
			format = ImGui::DataTypeGetInfo(dataType)->PrintFmt;

		const bool hovered = ImGui::ItemHoverable(frameRect, id, g.LastItemData.InFlags);
		const bool clicked = hovered && ImGui::IsMouseClicked(0, id);
        if (clicked)
		{
			ImGui::SetActiveID(id, window);
			ImGui::SetFocusID(id, window);
			ImGui::FocusWindow(window);
			g.ActiveIdUsingNavDirMask |= (1 << ImGuiDir_Left) | (1 << ImGuiDir_Right);
		}

		// Slider behavior
		ImRect grabRect;
		const bool valueChanged = ImGui::SliderBehavior(frameRect, id, dataType, pData, pMin, pMax, format, flags, &grabRect);
		if (valueChanged)
			ImGui::MarkItemEdited(id);

		// Draw frame
		const ImU32 frameCol = ImGui::GetColorU32(g.ActiveId == id ? ImGuiCol_FrameBgActive : ImGuiCol_FrameBg);
		float paddingX = grabRect.GetWidth() / 2.0f;
		float paddingY = grabRect.GetHeight() / 4.0f;
		ImRect bar(frameRect.Min.x + paddingX, grabRect.Min.y + paddingY, frameRect.Max.x - paddingX, grabRect.Max.y - paddingY);  
		ImGui::RenderNavHighlight(frameRect, id);
		ImGui::RenderFrame(bar.Min, bar.Max, frameCol, true, g.Style.FrameRounding);
		ImGui::RenderRectFilledRangeH(window->DrawList, bar, ImGui::GetColorU32(ImGuiCol_FrameBgHovered), 0.0f, fraction, style.FrameRounding);

		// Render grab
		if (grabRect.Max.x > grabRect.Min.x)
			window->DrawList->AddRectFilled(grabRect.Min, grabRect.Max, ImGui::GetColorU32(g.ActiveId == id ? ImGuiCol_SliderGrabActive : ImGuiCol_SliderGrab), style.GrabRounding);

		// Display value from format
		char valueBuf[64];
		const char* valueBufEnd = valueBuf + ImGui::DataTypeFormatString(valueBuf, IM_ARRAYSIZE(valueBuf), dataType, pData, format);
		if (g.LogEnabled)
			ImGui::LogSetNextTextDecoration("{", "}");
		ImGui::RenderTextClipped(frameRect.Min, frameRect.Max, valueBuf, valueBufEnd, NULL, ImVec2(0.5f, 0.5f));

		if (labelSize.x > 0.0f)
			ImGui::RenderText(ImVec2(frameRect.Max.x + style.ItemInnerSpacing.x, frameRect.Min.y + style.FramePadding.y), label);

		IMGUI_TEST_ENGINE_ITEM_INFO(id, label, g.LastItemData.StatusFlags | (temp_input_allowed ? ImGuiItemStatusFlags_Inputable : 0));
		return valueChanged;
	}

	// NOTE: Might want to change fraction calcultation to inside ImGuiSeekBar() depending on what is needed for future implementation.
	//       Fine for now
	bool ImGuiWidgets::ImGuiSeekBarFloat(const char* label, float* v, float vMax, const char* format, ImGuiSliderFlags flags)
	{
		float vMin = 0.0f;
		float fraction = *v / vMax;
		return ImGuiSeekBar(label, ImGuiDataType_Float, fraction, v, &vMin, &vMax, format, flags);
	}
	
	// NOTE: Might want to change fraction calcultation to inside ImGuiSeekBar() depending on what is needed for future implementation.
	//       Fine for now
	bool ImGuiWidgets::ImGuiSeekBarInt(const char* label, int* v, int vMax, const char* format, ImGuiSliderFlags flags)
	{
		int vMin = 0;
		float fraction = float(*v) / (float)vMax;
		return ImGuiSeekBar(label, ImGuiDataType_S32, fraction, v, &vMin, &vMax, format, flags);
	}

}