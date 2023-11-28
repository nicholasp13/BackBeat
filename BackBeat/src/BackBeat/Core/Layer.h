#pragma once

#include "Core.h"
#include "BackBeat/Events/Event.h"
namespace BackBeat {

	class BACKBEAT_API Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		// TODO: Remove DWORD as it is a Windows only data type
		virtual void OnUpdate(DWORD timeInterval) {} // For implementation later of a timer for performance and other reasons i.e. metronome, time adjustments
		virtual void OnEvent(Event& event) {}
		virtual void OnImGuiRender() {}

		inline const std::string& GetName() const { return m_DebugName; }

	protected:
		std::string m_DebugName;
	};

}