#include <BackBeat.h>

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