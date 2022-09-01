#pragma once

#include "Core.h"
#include "Events/ApplicationEvent.h"
#include "Log.h"

//Application class for applications using the BackBeat engine

namespace BackBeat {

	class BACKBEAT_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
	};

	// TO be defined in CLIENT
	Application* CreateApplication();

}