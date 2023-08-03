#include <BackBeat.h>

// Sample engine for development purpoes i.e. running and debugging

class ExampleLayer : public BackBeat::Layer
{
public:
	ExampleLayer()
		: Layer("Example")
	{
	}

	void OnUpdate() override
	{
		BB_CLIENT_INFO("ExampleLayer::Update");
	}

	void OnEvent(BackBeat::Event& event) override
	{
		BB_CLIENT_TRACE("{0}", event);
	}
};

class Sampler : public BackBeat::Application
{
public:
	Sampler()
	{
		// ----- LOGGING/DEBUGGING ----
		// PushLayer(new ExampleLayer());
		// PushOverlay(new BackBeat::ImGuiLayer());
		// BackBeat::FileReader::ReadFile("assets/audio/LearnToFly.mp3", nullptr);
		// BackBeat::FileReader::ReadFile("assets/audio/LearnToFly.wav", nullptr);
		 BackBeat::Player player = BackBeat::Player("assets/audio/LearnToFly.wav");
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