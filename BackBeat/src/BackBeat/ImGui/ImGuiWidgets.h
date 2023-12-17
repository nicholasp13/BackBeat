#pragma once

#include <imgui.h>
namespace BackBeat {

	class ImGuiWidgets
	{
	public:

		static bool ImGuiTimeline();

		// static bool ImGuiSeekBarInt(const char* label, int* val, int* max, const char* format);

		/**/
		static bool ImGuiSeekBar(const char* label, ImGuiDataType data_type, float fraction, void* p_data, const void* p_min, const void* p_max,
			const char* format, ImGuiSliderFlags flags);
		static bool ImGuiSeekBarFloat(const char* label, float* v, float v_max,
			const char* format, ImGuiSliderFlags flags);
		static bool ImGuiSeekBarInt(const char* label, int* v, int v_max, const char* format, ImGuiSliderFlags flags);
		/**/

	private:
		// static bool ImGuiSeekControl

		static inline ImVec2 Add(const ImVec2& lhs, const ImVec2& rhs) { return ImVec2(lhs.x + rhs.x, lhs.y + rhs.y); }

	};
}