/*
* Demonstrates a virtual Synth and basic application of BackBeat Engine. BackBeat can also support playback of WAV files 
* of certain properties. Future implentation of other sample rates, file types to be done later or as needed
*/

#include <BackBeat.h>
#include <BackBeat/Core/EntryPoint.h>

#include "MainLayer.h"

class Sampler : public BackBeat::Application
{
public:
	Sampler()
	{
		PushOverlay(new MainLayer(&this->GetWindow()));
	}

	~Sampler()
	{
	}

};

BackBeat::Application* BackBeat::CreateApplication()
{
	return new Sampler();
}