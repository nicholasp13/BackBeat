#include "Application.h"

namespace BackBeat {
	Application::Application()
	{
	}

	Application::~Application()
	{
	}

	void Application::Run()
	{
		WindowResizeEvent e(1200, 720);
		BB_CLIENT_TRACE(e);

		while (true);
	}
}