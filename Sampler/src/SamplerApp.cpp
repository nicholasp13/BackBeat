#include <BackBeat.h>
#include <BackBeat/Core/EntryPoint.h>

#include "MasterLayer.h"

class Sampler : public BackBeat::Application
{
public:
	Sampler()
	{
		// ----- LOGGING/DEBUGGING ----
		// PushOverlay(new BackBeat::ImGuiLayer());
		// BackBeat::FileReader::ReadFile("assets/audio/LearnToFly.mp3", nullptr);
		// BackBeat::FileReader::ReadFile("assets/audio/LearnToFly.wav", nullptr);
		// BackBeat::Player player = BackBeat::Player("assets/audio/LearnToFly.wav");
		// player.Play();
		// BackBeat::Player player = BackBeat::Player("assets/audio/Audio 1_39.wav");
	}

	~Sampler()
	{

	}

};

BackBeat::Application* BackBeat::CreateApplication()
{
	return new Sampler();
}

class ExampleLayer : public BackBeat::Layer
{
public:
	ExampleLayer()
		: Layer("Example")
	{
	}

	void OnEvent(BackBeat::Event& event) override
	{
		// BB_CLIENT_TRACE("{0}", event);
	}
};