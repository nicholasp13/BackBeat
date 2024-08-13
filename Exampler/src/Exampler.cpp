/* 
* Demonstrates application of BackBeat with Synth, Sampler, Recording, and Playback functionality. Playback only supports  
* WAV files with only 48k sample rate and supports all bit depths.
*/

#include <BackBeat.h>
#include <BackBeat/Core/EntryPoint.h>

#include "MainLayer.h"
namespace Exampler {

	class Exampler : public BackBeat::Application
	{
	public:
		Exampler()
		{
			PushLayer(new MainLayer(&this->GetWindow()));
		}

		~Exampler()
		{

		}

	};
}

BackBeat::Application* BackBeat::CreateApplication()
{
	return new Exampler::Exampler();
}