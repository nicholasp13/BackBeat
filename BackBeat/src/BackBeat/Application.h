#pragma once

#include "Core.h"

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