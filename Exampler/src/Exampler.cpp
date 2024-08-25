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
			m_Audio.Init(std::string("\\Exampler"));
			m_Audio.Start();

			PushLayer(new MainLayer(&this->GetWindow(), &m_Audio));
		}

		~Exampler()
		{
			
		}

	private:
		BackBeat::WindowsAudioSystem m_Audio;
	};
}

BackBeat::Application* BackBeat::CreateApplication()
{
	return new Exampler::Exampler();
}