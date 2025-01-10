#pragma once

#include <imgui.h>
#include <imgui_internal.h>

#include "BackBeat/Audio/Helpers/Snippets.h"
namespace BackBeat {

	namespace ImGuiWidgets
	{
		// TODO: Change from snake case to camel case
		void ImGuiTimeline(const char* label, const float* values, int values_count, int values_offset, const char* overlay_text, 
			float scaleMin, float scaleMax, ImVec2 graph_size, int stride, float fraction);
		void ImGuiTimeline(const char* label, float (*values_getter)(void* data, int idx), void* data, 
			int values_count, int values_offset, const char* overlay_text, float scaleMin, float scale_max, ImVec2 graph_size, float fraction);
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
		int ImGuiPlotTimeline(const char* label, float (*values_getter)(void* data, int idx), void* data, 
			int values_count, int values_offset, const char* overlay_text, float scaleMin, float scaleMax, const ImVec2& size_arg, float fraction);
		
		static inline ImVec2 Add(const ImVec2& lhs, const ImVec2& rhs) { return ImVec2(lhs.x + rhs.x, lhs.y + rhs.y); }
	}
}