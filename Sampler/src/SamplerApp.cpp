#include <BackBeat.h>
#include <BackBeat/Core/EntryPoint.h>

#include "MasterLayer.h"

class Sampler : public BackBeat::Application
{
public:
	Sampler()
	{
		PushOverlay(new MasterLayer(&this->GetWindow()));
	}

	~Sampler()
	{
	}

};

BackBeat::Application* BackBeat::CreateApplication()
{
	return new Sampler();
}