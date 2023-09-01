#pragma once

#include "BackBeat/Core/Layer.h"

namespace BackBeat {

	class BACKBEAT_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach();
		virtual void OnDetach();
		virtual void OnEvent(Event& event);
		virtual void Begin();
		virtual void End();

		void SetDarkThemeColors();

	private:
		float m_Time = 0.0f;
	};
}