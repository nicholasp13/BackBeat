#pragma once

#ifdef BB_PLATFORM_WINDOWS

extern BackBeat::Application* BackBeat::CreateApplication();

int main(int argc, char** argv)
{
	printf("BackBeat Engine");
	auto app = BackBeat::CreateApplication();
	app->Run();
	delete app;
 
}

#endif // 
