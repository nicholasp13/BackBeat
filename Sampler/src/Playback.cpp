/** Use this for play bar ( from ImGui::ShowDemoWindow() )
* ImGui::ProgressBar(progress, ImVec2(0.0f, 0.0f));
* ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
* ImGui::Text("Progress Bar");
*
* float progress_saturated = IM_CLAMP(progress, 0.0f, 1.0f);
* char buf[32];
* sprintf(buf, "%d/%d", (int)(progress_saturated * 1753), 1753);
* ImGui::ProgressBar(progress, ImVec2(0.f, 0.f), buf);
* ImGui::TreePop();
*/ 

#include "Playback.h"

