#include "bbpch.h"

#include <imgui.h>
#include <imgui_internal.h>
#include "ImGuiWidgets.h"

namespace BackBeat {

	// Taken from ImGui
	struct ImGuiPlotArrayGetterData
	{
		const float* Values;
		int Stride;

		ImGuiPlotArrayGetterData(const float* values, int stride) { Values = values; Stride = stride; }
	};

	// Taken from ImGui
	static float Plot_ArrayGetter(void* data, int idx)
	{
		ImGuiPlotArrayGetterData* plot_data = (ImGuiPlotArrayGetterData*)data;
		const float v = *(const float*)(const void*)((const unsigned char*)plot_data->Values + (size_t)idx * plot_data->Stride);
		return v;
	}

	// Modeled after ImGui::PlotLines()
	void ImGuiWidgets::ImGuiTimeline(const char* label, const float* values, int valuesCount, int valuesOffset, const char* overlayText,
		float scaleMin, float scaleMax, ImVec2 graphSize, int stride, float fraction)
	{
		ImGuiPlotArrayGetterData data(values, stride);
		ImGuiPlotTimeline(label, &Plot_ArrayGetter, (void*)&data, valuesCount, valuesOffset, overlayText, scaleMin, scaleMax, graphSize, fraction);
	}

	// Modeled after ImGui::PlotLines()
	void ImGuiWidgets::ImGuiTimeline(const char* label, float (*values_getter)(void* data, int idx), void* data,
		int valuesCount, int valuesOffset, const char* overlayText, float scaleMin, float scaleMax, ImVec2 graphSize, float fraction)
	{
		ImGuiPlotTimeline(label, &Plot_ArrayGetter, (void*)&data, valuesCount, valuesOffset, overlayText, scaleMin, scaleMax, graphSize, fraction);
	}

	// returns int but it is currently not used and subject to change
	// Modeled after ImGui::PlotEx()
	int ImGuiWidgets::ImGuiPlotTimeline(const char* label, float (*valuesGetter)(void* data, int idx), void* data,
		int valuesCount, int valuesOffset, const char* overlayText, float scaleMin, float scaleMax, const ImVec2& sizeArg, float fraction)
	{
		ImGuiContext& g = *GImGui;
		ImGuiWindow* window = ImGui::GetCurrentWindow();
		if (window->SkipItems)
			return -1;

		const ImGuiStyle& style = g.Style;
		const ImGuiID id = window->GetID(label);

		const ImVec2 labelSize = ImGui::CalcTextSize(label, NULL, true);
		const ImVec2 frameSize = ImGui::CalcItemSize(sizeArg, ImGui::CalcItemWidth(), labelSize.y + style.FramePadding.y * 2.0f);

		const ImRect frameRect(window->DC.CursorPos, Add(window->DC.CursorPos, frameSize));
		const ImRect innerRect(Add(frameRect.Min, style.FramePadding), Sub(frameRect.Max, style.FramePadding));
		const ImRect totalRect(frameRect.Min, Add(frameRect.Max, ImVec2(labelSize.x > 0.0f ? style.ItemInnerSpacing.x + labelSize.x : 0.0f, 0)));
		ImGui::ItemSize(totalRect, style.FramePadding.y);
		if (!ImGui::ItemAdd(totalRect, 0, &frameRect))
			return -1;
		const bool hovered = ImGui::ItemHoverable(frameRect, id, g.LastItemData.InFlags);

		// Determine scale from values if not specified
		if (scaleMin == FLT_MAX || scaleMax == FLT_MAX)
		{
			float vMin = FLT_MAX;
			float vMax = -FLT_MAX;
			for (int i = 0; i < valuesCount; i++)
			{
				const float v = valuesGetter(data, i);
				if (v != v) // Ignore NaN values
					continue;
				vMin = ImMin(vMin, v);
				vMax = ImMax(vMax, v);
			}
			if (scaleMin == FLT_MAX)
				scaleMin = vMin;
			if (scaleMax == FLT_MAX)
				scaleMax = vMax;
		}

		ImGui::RenderFrame(frameRect.Min, frameRect.Max, ImGui::GetColorU32(ImGuiCol_FrameBg), true, style.FrameRounding);

		const int valuesCountMin = 2;
		int idxHovered = -1;
		if (valuesCount >= valuesCountMin)
		{
			int resWidth = ImMin((int)frameSize.x, valuesCount) + -1;
			int itemCount = valuesCount + -1;

			const float tStep = 1.0f / (float)resWidth;
			const float invScale = (scaleMin == scaleMax) ? 0.0f : (1.0f / (scaleMax - scaleMin));

			float v0 = valuesGetter(data, (0 + valuesOffset) % valuesCount);
			float t0 = 0.0f;
			ImVec2 tp0 = ImVec2(t0, 1.0f - ImSaturate((v0 - scaleMin) * invScale)); // Point in the normalized space of our target rectangle

			const ImU32 colBase = ImGui::GetColorU32(ImGuiCol_PlotLines);
			const ImU32 colHovered = ImGui::GetColorU32(ImGuiCol_PlotLinesHovered);

			for (int n = 0; n < resWidth; n++)
			{
				const float t1 = t0 + tStep;
				const int v1Idx = (int)(t0 * itemCount + 0.5f);
				IM_ASSERT(v1Idx >= 0 && v1Idx < valuesCount);
				const float v1 = valuesGetter(data, (v1Idx + valuesOffset + 1) % valuesCount);
				const ImVec2 tp1 = ImVec2(t1, 1.0f - ImSaturate((v1 - scaleMin) * invScale));

				ImVec2 pos0 = ImLerp(innerRect.Min, innerRect.Max, tp0);
				ImVec2 pos1 = ImLerp(innerRect.Min, innerRect.Max, tp1);
				window->DrawList->AddLine(pos0, pos1, idxHovered == v1Idx ? colHovered : colBase);

				t0 = t1;
				tp0 = tp1;
			}
		}

		// Text overlay
		if (overlayText)
			ImGui::RenderTextClipped(ImVec2(frameRect.Min.x + style.FramePadding.x, frameRect.Min.y + style.FramePadding.y), frameRect.Max, overlayText, NULL, NULL, ImVec2(0.0f, 0.0f));

		if (labelSize.x > 0.0f)
			ImGui::RenderText(ImVec2(frameRect.Max.x + style.ItemInnerSpacing.x, innerRect.Min.y), label);

		// Progress overlay tracker
		fraction = ImSaturate(fraction);
		const float progressWidth = 2.0f;
		const float progressHeight = frameRect.GetHeight();
		const float xPos = frameRect.Min.x + (frameRect.GetWidth() * fraction);
		const float yPos = frameRect.Min.y;
		const ImRect progressMarker(ImVec2(xPos, yPos), ImVec2(xPos, yPos + progressHeight));
		window->DrawList->AddLine(progressMarker.Min, progressMarker.Max, ImGui::GetColorU32(ImGuiCol_SliderGrab), progressWidth);

		// Return hovered index or -1 if none are hovered.
		return idxHovered;
	}
}