/*
* Demonstrates application of BackBeat Engine with a Synth and Playback functionality. Playback only supports  
* WAV files with only 48k sample rate and supports all bit depths except 24 bit.
*/

#include <BackBeat.h>
#include <BackBeat/Core/EntryPoint.h>

#include "MainLayer.h"
class Sampler : public BackBeat::Application
{
public:
	Sampler()
	{
		PushLayer(new MainLayer(&this->GetWindow()));
	}

	~Sampler()
	{
	}

};

BackBeat::Application* BackBeat::CreateApplication()
{
	return new Sampler();
}