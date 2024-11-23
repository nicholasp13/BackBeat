#include "bbpch.h"

#include <imgui.h>
#include <imgui_internal.h>
#include "ImGuiWidgets.h"
namespace BackBeat {

	// NOTE: Only handles int, may implement other data types later
	//       Flags not used, may remove
	// COLORS:
	//     - Frame rectangle  - ImGui::GetColorU32(g.ActiveId == id ? ImGuiCol_FrameBgActive : ImGuiCol_FrameBg)
	//     - Active rectangle - ImGuiCol_SliderGrab
	bool ImGuiWidgets::ImGuiTrackEditor(const char* label, int* lowerV, int* upperV, const int* pMin, const int* pMax,
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
		const ImRect frameRect(window->DC.CursorPos, Add(window->DC.CursorPos, ImVec2(w, labelSize.y + style.FramePadding.y * 2.0f)));
		const ImRect totalRect(frameRect.Min, Add(frameRect.Max, ImVec2(labelSize.x > 0.0f ? style.ItemInnerSpacing.x + labelSize.x : 0.0f, 0.0f)));

		ImGui::ItemSize(totalRect, style.FramePadding.y);
		if (!ImGui::ItemAdd(totalRect, id, &frameRect, 0))
			return false;

		// Default format string when passing NULL
		if (format == NULL)
			format = ImGui::DataTypeGetInfo(ImGuiDataType_S32)->PrintFmt;

		const bool hovered = ImGui::ItemHoverable(frameRect, id, g.LastItemData.InFlags);
		const bool clicked = hovered && ImGui::IsMouseClicked(0, id);
		if (clicked)
		{
			ImGui::SetActiveID(id, window);
			ImGui::SetFocusID(id, window);
			ImGui::FocusWindow(window);
			g.ActiveIdUsingNavDirMask |= (1 << ImGuiDir_Left) | (1 << ImGuiDir_Right);
		}

		ImRect grabLower;
		ImRect grabUpper;
		bool valueChanged = ImGuiDoubleSliderBehavior(frameRect, id, lowerV, upperV, *pMin, *pMax, 
			&grabLower, &grabUpper);

		if (valueChanged)
			ImGui::MarkItemEdited(id);

		// Calculate upper and lower bounds frame
		ImRect activeTrack = ImRect(grabLower.Min, grabUpper.Max);

		// Draw frame
		const ImU32 frameCol = ImGui::GetColorU32(g.ActiveId == id ? ImGuiCol_FrameBgActive : ImGuiCol_FrameBg);
		ImGui::RenderFrame(frameRect.Min, frameRect.Max, frameCol, true, g.Style.FrameRounding);
		ImGui::RenderFrame(activeTrack.Min, activeTrack.Max, ImGui::GetColorU32(ImGuiCol_SliderGrab), true, g.Style.FrameRounding);

		// Display value from format
		char valueBuf[64] = {};
		const char* valueBufEnd = valueBuf + ImGui::DataTypeFormatString(valueBuf, IM_ARRAYSIZE(valueBuf), ImGuiDataType_S32, lowerV, format);
		if (g.LogEnabled)
			ImGui::LogSetNextTextDecoration("{", "}");
		ImGui::RenderTextClipped(frameRect.Min, frameRect.Max, valueBuf, valueBufEnd, NULL, ImVec2(0.5f, 0.5f));

		IMGUI_TEST_ENGINE_ITEM_INFO(id, label, g.LastItemData.StatusFlags | (temp_input_allowed ? ImGuiItemStatusFlags_Inputable : 0));
		return valueChanged;
	}

	// NOTE: Currently unused and unfinished as it will be worked on after serialization is finished
	bool ImGuiWidgets::ImGuiTrackSnipper(const char* label, Snippets* snippets, const int* pMin, const int* pMax, 
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
		const ImRect frameRect(window->DC.CursorPos, Add(window->DC.CursorPos, ImVec2(w, labelSize.y + style.FramePadding.y * 2.0f)));
		const ImRect totalRect(frameRect.Min, Add(frameRect.Max, ImVec2(labelSize.x > 0.0f ? style.ItemInnerSpacing.x + labelSize.x : 0.0f, 0.0f)));

		ImGui::ItemSize(totalRect, style.FramePadding.y);
		if (!ImGui::ItemAdd(totalRect, id, &frameRect, 0))
			return false;

		// Default format string when passing NULL
		if (format == NULL)
			format = ImGui::DataTypeGetInfo(ImGuiDataType_S32)->PrintFmt;

		const bool hovered = ImGui::ItemHoverable(frameRect, id, g.LastItemData.InFlags);
		const bool clicked = hovered && ImGui::IsMouseClicked(0, id);
		if (clicked)
		{
			ImGui::SetActiveID(id, window);
			ImGui::SetFocusID(id, window);
			ImGui::FocusWindow(window);
			g.ActiveIdUsingNavDirMask |= (1 << ImGuiDir_Left) | (1 << ImGuiDir_Right);
		}

		return true;
	}

	// NOTE: - Future implementation might want the track to keep playing while visually the seek bar changes
	//           and only when the bar is released then the position is changed. (This does not work as either
	//           the bar does not update while the track plays but does change only when released OR the
	//           bar does not move or change with the track position) The following example code shows an
	//           implementation that avoids the constant audio playing while the user is using the seekbar().
	//  ---- Example Code ----
	// if (BackBeat::ImGuiWidgets::ImGuiSeekBarInt("##", &position, m_Player->GetSize(), "", ImGuiSliderFlags(0)))
	// {
	//     if (m_Player->IsPlaying())
	//	   {
	//	       m_Player->Pause();
	//	       wasPlaying = true;
	//	   }
	//	   m_Player->SetPosition(position);
	// }
	// if (ImGui::IsItemDeactivated() && wasPlaying)
	// {
	//     m_Player->Play();
	//	   wasPlaying = false;
	// }
	//  ----------------------
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

	// NOTE: Only handles int, may implement other data types later
	//       Might want to create way to set padding between lower and upper border
	//       Might want to make the widget stop when the user crosses the upper and lower borders
	bool ImGuiWidgets::ImGuiDoubleSliderBehavior(ImRect frame, ImGuiID id, int* lowerV, int* upperV, const int vMin, const int vMax,
		ImRect* grabLower, ImRect* grabUpper)
	{
		ImGuiContext& g = *GImGui;
		const ImGuiStyle& style = g.Style;

		// Calculating bounds
		const float vRange = (float)(vMin < vMax ? vMax - vMin : vMin - vMax);
		const float grabPadding = 2.0f;
		const float sliderSize = (frame.Max.x - frame.Min.x);
		const float sliderMin = frame.Min.x;
		const float sliderMax = frame.Max.x;

		// Calcultating upper and lower positions
		float lowerTruePos = ((float)(*lowerV - vMin) / (float)(vMax - vMin)) * sliderSize + sliderMin;
		float upperTruePos = ((float)(*upperV - vMin) / (float)(vMax - vMin)) * sliderSize + sliderMin;

		const bool hovered = ImGui::ItemHoverable(frame, id, g.LastItemData.InFlags);
		if (hovered)
			g.MouseCursor = ImGuiMouseCursor_TextInput;

		// Slider interaction
		bool valueChanged = false;
		bool lowerActive = false;
		bool upperActive = false;
		float clickedPos = 0.0f;
		if (g.ActiveId == id)
		{
			if (g.ActiveIdSource == ImGuiInputSource_Mouse)
			{
				if (!g.IO.MouseDown[0])
				{
					ImGui::ClearActiveID();
				}
				else
				{
					float mouseTruePos = g.IO.MousePos.x;
					float lowerDist = std::abs(mouseTruePos - lowerTruePos);
					float upperDist = std::abs(mouseTruePos - upperTruePos);

					if ((std::min)(lowerDist, upperDist) == lowerDist)
					{
						if (g.ActiveIdIsJustActivated)
						{
							bool clickedNearGrab = (mouseTruePos >= lowerTruePos - 2.0f) && (mouseTruePos <= lowerTruePos + 2.0f);
							g.SliderGrabClickOffset = clickedNearGrab ? mouseTruePos - lowerTruePos : 0.0f;
						}
						lowerActive = true;
					}
					else
					{
						if (g.ActiveIdIsJustActivated)
						{
							
							bool clickedNearGrab = (mouseTruePos >= upperTruePos - 2.0f) && (mouseTruePos <= upperTruePos + 2.0f);
							g.SliderGrabClickOffset = clickedNearGrab ? mouseTruePos - upperTruePos : 0.0f;
						}
						upperActive = true;
					}
					clickedPos = (mouseTruePos - g.SliderGrabClickOffset - sliderMin) / sliderSize;
					clickedPos = clickedPos > 1.0f ? 1.0f : clickedPos < 0.0f ? 0.0f : clickedPos;
					valueChanged = true;
				}
			}
		}
		if (valueChanged) 
		{
			int newVal = (int)(clickedPos * ((float)vMax - (float)vMin) + (float)vMin);
			if (lowerActive)
				*lowerV = newVal;
			else if (upperActive)
				*upperV = newVal;
		}

		// Sets bounds for rendering
		const int padding = 64;
		*lowerV = *lowerV >= *upperV ? *upperV - padding : *lowerV < vMin ? vMin : *lowerV;
		*upperV = *upperV <= *lowerV ? *lowerV + padding : *upperV > vMax ? vMax : *upperV;
		lowerTruePos = (float(*lowerV) - float(vMin)) / float(vMax - vMin) * sliderSize + sliderMin;
		upperTruePos = (float(*upperV) - float(vMin)) / float(vMax - vMin) * sliderSize + sliderMin;
		*grabLower = ImRect(ImVec2(lowerTruePos, frame.Min.y),
			ImVec2(lowerTruePos, frame.Max.y));
		*grabUpper = ImRect(ImVec2(upperTruePos, frame.Min.y),
			ImVec2(upperTruePos, frame.Max.y));

		return valueChanged;
	}
}