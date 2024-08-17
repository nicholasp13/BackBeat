#pragma once

#include "BackBeat/Audio/Renderer/RenderInfo.h"
#include "BackBeat/Audio/IO/AudioProcessor.h"
#include "BackBeat/Audio/Instruments/Synth/AudioEngine/AudioEngine.h"
namespace BackBeat {

	class SynthProcessor : public AudioProcessor
	{
	public:
		SynthProcessor(AudioProps props, std::shared_ptr<AudioEngine> engine, UUID id);
		~SynthProcessor();

		virtual void ProcessSamples(unsigned int numSamples, unsigned int sampleRate, unsigned int numChannels) override;

		virtual AudioProps GetProperties() override { return m_Props; }
		virtual AudioBus* GetBus() override { return &m_Bus; }
		virtual UUID GetID() override { return m_ID; }
		virtual void* GetOutputBuffer() override { return m_Output; }
		virtual bool IsOn() override { return m_On; }

		void Off();

		inline void On() { m_On = true; }
		inline AudioBuffer* GetBuffer() { return m_Bus.GetBuffer(); }
		inline std::shared_ptr<RenderInfo> GetInfo() { return m_Info; }

	private:
		static const unsigned int s_BufferSize = 100000;

		bool m_On;
		float m_Output[s_BufferSize] = {};
		AudioProps m_Props;
		AudioBus m_Bus;
		UUID m_ID;
		
		std::shared_ptr<RenderInfo> m_Info;
		std::shared_ptr<AudioEngine> m_Engine;
	};

}