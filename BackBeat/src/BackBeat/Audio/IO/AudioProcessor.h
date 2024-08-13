#pragma once

// Abstract class that handles the input and output of audio busses from audio output sources like file players
// and synths.
#include "AudioBus.h"
#include "BackBeat/Core/UUID.h"
namespace BackBeat {

	class AudioProcessor
	{
	public:
		virtual void ProcessSamples(unsigned int numSamples, unsigned int sampleRate, unsigned int numChannels) {};		
		virtual AudioProps GetProperties() { return AudioProps(); };
		virtual AudioBus* GetBus() { return nullptr; };
		virtual UUID GetID() { return UUID(0); }
		virtual void* GetOutputBuffer() { return nullptr; };
		virtual bool IsOn() { return false; }

	};
}