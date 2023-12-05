#pragma once

#include "Track.h"
#include "BackBeat/Audio/IO/AudioProcessor.h"
namespace BackBeat {

	class PlayerProcessor : public AudioProcessor
	{
	public:
		PlayerProcessor();
		~PlayerProcessor();

		virtual void ProcessSamples(unsigned int numSamples, unsigned int sampleRate, unsigned int numChannels);

		void PlayTrack(Track* track);

		virtual AudioProps GetProperties() { return m_Track->GetProps(); };
		virtual AudioBus* GetBus() { return nullptr; };
		virtual void* GetOutputBuffer() { return m_Output; };
		virtual bool IsOn() { return m_On; }
	
		void On() { m_On = true; }
		void Off() { m_On = false; }

	private:
		bool m_On;
		unsigned int m_BufferSize;
		byte* m_Output;
		Track* m_Track;
	};
}