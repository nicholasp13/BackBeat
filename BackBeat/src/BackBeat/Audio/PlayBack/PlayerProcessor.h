#pragma once

#include "Track.h"
#include "BackBeat/Audio/IO/AudioProcessor.h"
namespace BackBeat {

	class PlayerProcessor : public AudioProcessor
	{
	public:
		PlayerProcessor();
		~PlayerProcessor();

		virtual void ProcessSamples(unsigned int numSamples, unsigned int sampleRate, unsigned int numChannels) override;

		void PlayTrack(Track* track);

		inline virtual AudioProps GetProperties() override { return m_Track->GetProps(); };
		inline virtual AudioBus* GetBus() override { return nullptr; };
		inline virtual void* GetOutputBuffer() override { return m_Output; };
		inline virtual bool IsOn() override { return m_On; }
	
		inline void On() { m_On = true; }
		inline void Off() { m_On = false; }

	private:
		bool m_On;
		unsigned int m_BufferSize;
		byte* m_Output;
		Track* m_Track;

		void MonoToStereo(unsigned int numBytes, unsigned int bitDepth, byte* mBuffer, byte* sBuffer);
		void StereoToMono(unsigned int numBytes, unsigned int bitDepth, byte* sBuffer, byte* mBuffer);
	};
}