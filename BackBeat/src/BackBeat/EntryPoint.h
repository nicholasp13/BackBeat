#pragma once

#ifdef BB_PLATFORM_WINDOWS

// Creates entry point for the application window

extern BackBeat::Application* BackBeat::CreateApplication();

int main(int argc, char** argv)
{
	BackBeat::Log::Init();
	BB_CORE_WARN("Initialized Log");
	int a = 69;
	BB_CLIENT_INFO("Var={0} nice", a);

	auto app = BackBeat::CreateApplication();
	app->Run();
	delete app;
 
}

#endif // 
