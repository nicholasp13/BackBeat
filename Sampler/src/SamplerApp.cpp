#include <BackBeat.h>

// Sample engine for development purpoes i.e. running and debugging

class Sampler : public BackBeat::Application
{
public:
	Sampler()
	{

	}

	~Sampler()
	{

	}

};

BackBeat::Application* BackBeat::CreateApplication()
{
	return new Sampler();
}