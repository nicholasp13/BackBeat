#pragma once

#include <imgui.h>
#include <imgui_internal.h>

#include "BackBeat/Audio/Helpers/Snippets.h"
namespace BackBeat {

	namespace ImGuiWidgets
	{

		bool ImGuiTimeline();
		bool ImGuiTrackEditor(const char* label, int* lowerV, int* upperV, const int* pMin, const int* pMax,
			const char* format, ImGuiSliderFlags flags);
		bool ImGuiTrackSnipper(const char* label, Snippets* snippets, const int* pMin, const int* pMax, 
			const char* format, ImGuiSliderFlags flags);
		bool ImGuiSeekBar(const char* label, ImGuiDataType data_type, float fraction, void* pData, const void* pMin, const void* pMax,
			const char* format, ImGuiSliderFlags flags);
		bool ImGuiSeekBarFloat(const char* label, float* v, float vMax,
			const char* format, ImGuiSliderFlags flags);
		bool ImGuiSeekBarInt(const char* label, int* v, int vMax, const char* format, ImGuiSliderFlags flags);
		bool ImGuiDoubleSliderBehavior(ImRect frame, ImGuiID id, int* lowerV, int* upperV, const int vMin, const int vMax,
			ImRect* grabLower, ImRect* grabUpper);

		static inline ImVec2 Add(const ImVec2& lhs, const ImVec2& rhs) { return ImVec2(lhs.x + rhs.x, lhs.y + rhs.y); }

	}
}