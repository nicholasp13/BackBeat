#pragma once

// Dummy Entity class used to test custom ImGui::Widgets without ruining current GUI

#include "Entity.h"
namespace Exampler {

	class Dummy : public Entity
	{
	public:
		Dummy();
		~Dummy();

		virtual void ImGuiRender() override;

		inline virtual std::string GetName() override { return m_Name; }

	private:
		bool m_Open;
		std::string m_Name;
	};

}