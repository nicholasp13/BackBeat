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
		virtual AudioProps GetProperties() override;

		void On();
		void PlayTrack(std::shared_ptr<Track> track);
		void ClearTrack();

		inline virtual AudioBus* GetBus() override { return nullptr; };
		inline virtual UUID GetID() override { return m_ID; }
		inline virtual void* GetOutputBuffer() override { return m_Output; };
		inline virtual bool IsOn() override { return m_On; }
	
		inline void Off() { m_On = false; }

	private:
		bool m_On;
		unsigned int m_BufferSize;
		UUID m_ID;
		byte* m_Output;
		std::shared_ptr<Track> m_Track;

	private:
		void MonoToStereo(unsigned int numBytes, unsigned int bitDepth, byte* mBuffer, byte* sBuffer);
		void StereoToMono(unsigned int numBytes, unsigned int bitDepth, byte* sBuffer, byte* mBuffer);
	};
}