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
		PushLayer(new ExampleLayer());
	}

	~Sampler()
	{

	}

};

BackBeat::Application* BackBeat::CreateApplication()
{
	return new Sampler();
}