#pragma once

#include "BackBeat/Audio/IO/RenderInfo.h"
#include "BackBeat/Audio/IO/AudioProcessor.h"
#include "BackBeat/Audio/Synth/AudioEngine/AudioEngine.h"
namespace BackBeat {

	class SynthProcessor : public AudioProcessor
	{
	public:
		SynthProcessor(AudioProps props, std::shared_ptr<AudioEngine> engine);
		~SynthProcessor();

		virtual void ProcessSamples(unsigned int numSamples, unsigned int sampleRate, unsigned int numChannels);

		virtual AudioProps GetProperties() { return m_Props; }
		virtual AudioBus* GetBus() { return &m_Bus; }
		virtual void* GetOutputBuffer() { return m_Output; }
		virtual bool IsOn() { return m_On; }

		void Off();

		inline void On() { m_On = true; }
		inline AudioBuffer* GetBuffer() { return m_Bus.GetBuffer(); }
		inline std::shared_ptr<RenderInfo> GetInfo() { return m_Info; }

	private:
		bool m_On;
		float* m_Output;

		AudioProps m_Props;
		AudioBus m_Bus;
		
		std::shared_ptr<RenderInfo> m_Info;
		std::shared_ptr<AudioEngine> m_Engine;
	};

}