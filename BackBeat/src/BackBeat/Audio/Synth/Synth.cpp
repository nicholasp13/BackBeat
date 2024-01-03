#include "bbpch.h"

#include "Synth.h"
namespace BackBeat {

	Synth::Synth()
	{
		InitSynth();
	}

	Synth::~Synth()
	{

	}

	void Synth::Start()
	{
		m_Running = true;
		m_Proc->On();
	}

	void Synth::Stop()
	{
		m_Running = false;
		m_Proc->Off();
		m_Engine->Stop();
	}

	void Synth::InitSynth()
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
			.format =      format,
			.numChannels = numChannels,
			.sampleRate =  sampleRate,
			.byteRate =    byteRate,
			.blockAlign =  blockAlign,
			.bitDepth =    bitDepth, 
			.fileSize =    0
		};

		m_Engine = std::make_shared<AudioEngine>(m_Props);
		m_Proc = std::make_shared<SynthProcessor>(m_Props, m_Engine);
		m_Handler = std::make_shared<SynthEventHandler>(m_Proc->GetInfo());
		m_MIDIInput = std::make_shared<MIDIInputHandler>(m_Proc->GetInfo());
		m_Params = std::make_shared<SynthParameters>(m_Engine->GetParam(), m_Handler->GetParams());
	}

	void Synth::Run()
	{
		
	}
}