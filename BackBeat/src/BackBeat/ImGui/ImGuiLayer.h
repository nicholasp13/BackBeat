#pragma once

#include "BackBeat/Core/Layer.h"
#include "BackBeat/Core/Application.h"

namespace BackBeat {

	class BACKBEAT_API ImGuiLayer : public Layer 
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach();
		virtual void OnDetach();
		virtual void OnUpdate();
		virtual void OnEvent(Event& event);

		void SetDarkThemeColors();

	private:
		float m_Time = 0.0f;
	};
}