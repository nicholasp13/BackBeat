#pragma once

#include <imgui.h>
#include <imgui_internal.h>

#include "BackBeat/Audio/Helpers/Snippets.h"
namespace BackBeat {

	namespace ImGuiWidgets
	{

		void ImGuiTimeline(const char* label, const float* values, int valuesCount, int valuesOffset, const char* overlayText, 
			float scaleMin, float scaleMax, ImVec2 graphSize, int stride, float fraction);
		void ImGuiTimeline(const char* label, float (*valuesGetter)(void* data, int idx), void* data, 
			int valuesCount, int valuesOffset, const char* overlayText, float scaleMin, float scaleMax, ImVec2 graphSize, float fraction);
		
		bool ImGuiTrackEditor(const char* label, int* lowerV, int* upperV, const int* pMin, const int* pMax,
			const char* format, ImGuiSliderFlags flags);
		
		bool ImGuiTrackSnipper(const char* label, Snippets* snippets, const int* pMin, const int* pMax, 
			const char* format, ImGuiSliderFlags flags);
		
		bool ImGuiSeekBar(const char* label, ImGuiDataType dataType, float fraction, void* pData, const void* pMin, const void* pMax,
			const char* format, ImGuiSliderFlags flags);
		bool ImGuiSeekBarFloat(const char* label, float* v, float vMax,
			const char* format, ImGuiSliderFlags flags);
		bool ImGuiSeekBarInt(const char* label, int* v, int vMax, const char* format, ImGuiSliderFlags flags);
		bool ImGuiSeekBarUnsignedInt(const char* label, unsigned int* v, unsigned int vMax, const char* format, ImGuiSliderFlags flags);
		
		bool ImGuiDoubleSliderBehavior(ImRect frame, ImGuiID id, int* lowerV, int* upperV, const int vMin, const int vMax,
			ImRect* grabLower, ImRect* grabUpper);
		int ImGuiPlotTimeline(const char* label, float (*valuesGetter)(void* data, int idx), void* data, 
			int valuesCount, int valuesOffset, const char* overlayText, float scaleMin, float scaleMax, const ImVec2& sizeArg, float fraction);

		bool ImGuiPad(const char* label, const ImVec2& sizeArg);
		bool ImGuiPad(const char* label, bool active, const ImVec2& sizeArg);
		bool ImGuiPadEx(const char* label, bool active, const ImVec2& sizeArg, ImGuiButtonFlags flags);

		static inline ImVec2 Add(const ImVec2& lhs, const ImVec2& rhs) { return ImVec2(lhs.x + rhs.x, lhs.y + rhs.y); }
		static inline ImVec2 Sub(const ImVec2& lhs, const ImVec2& rhs) { return ImVec2(lhs.x - rhs.x, lhs.y - rhs.y); }

	}
}