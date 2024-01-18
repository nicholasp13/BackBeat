#include "bbpch.h"

#include "Sampler.h"
namespace BackBeat {

	Sampler::Sampler()
	{
		InitSampler();
	}

	Sampler::~Sampler()
	{

	}

	void Sampler::Start()
	{
		m_Running = true;
		m_Processor->On();
	}

	void Sampler::Stop()
	{
		m_Running = false;
		m_Processor->Off();
		m_Engine->Stop();
	}

	void Sampler::InitSampler()
	{
		m_Running = false;
		const unsigned short format = 3;
		const unsigned short numChannels = 2;
		const unsigned long sampleRate = 48000;
		const unsigned long byteRate = 384000;
		const unsigned short blockAlign = 8;
		const unsigned short bitDepth = 32;
		m_Props = {
			.bigEndian = Audio::IsBigEndian(),
			.format = format,
			.numChannels = numChannels,
			.sampleRate = sampleRate,
			.byteRate = byteRate,
			.blockAlign = blockAlign,
			.bitDepth = bitDepth,
			.fileSize = 0
		};
		m_Engine = std::make_shared<SamplerEngine>(m_Props);
		m_Processor = std::make_shared<SamplerProcessor>(m_Props, m_Engine);
		m_Programmer = std::make_shared<SampleProgrammer>(m_Props);
		m_MIDIInput = std::make_shared<MIDIInputHandler>(m_Processor->GetInfo());
		m_Handler = std::make_shared<SamplerEventHandler>();
		
		// SamplerEventHandler Key controls are 1 - 9, 0 keys
		const KeyCode code = Key::D1;
		for (unsigned int i = 0; i < SamplerMaxVoices; i++) {
			auto newPad = std::make_shared<SamplerPad>(m_MIDIInput, m_Engine, i);
			m_Programmer->AddSamplerPad(newPad);
			if (i < SamplerMaxVoices - 1)
				m_Handler->AddSamplerPad(newPad, code + i);
			else
				m_Handler->AddSamplerPad(newPad, Key::D0);
		}
	}

	void Sampler::Run()
	{

	}

}