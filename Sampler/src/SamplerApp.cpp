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
		// BackBeat::Player player = BackBeat::Player();
		// std::string result1 = 
		//	BackBeat::FileReader::ReadFile("assets/audio/LearnToFly.mp3" );
		// BB_CLIENT_TRACE("{0}", result1);
		// std::string result2 =
		//	BackBeat::FileReader::ReadFile("assets/audio/Audio 1_39.wav");
		// BB_CLIENT_TRACE("{0}", result2);
	}

	~Sampler()
	{

	}

};

BackBeat::Application* BackBeat::CreateApplication()
{
	return new Sampler();
}