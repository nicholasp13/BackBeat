#pragma once

#include "BackBeat/Audio/MIDI/MIDIInputHandler.h"
#include "BackBeat/Audio/Sampler/SamplerEngine/SamplerEngine.h"
#include "BackBeat/Audio/Sampler/SamplerIO/SampleProgrammer.h"
#include "BackBeat/Audio/Sampler/SamplerIO/SamplerEventHandler.h"
#include "BackBeat/Audio/Sampler/SamplerIO/SamplerProcessor.h"
namespace BackBeat {

	class Sampler
	{
	public:
		Sampler();
		~Sampler();

		void Start();
		void Stop();

		inline bool IsRunning() { return m_Running; }
		inline AudioProps GetProps() { return m_Props; }
		inline std::shared_ptr<SamplerEventHandler> GetEventHandler() { return m_Handler; }
		inline std::shared_ptr<MIDIInputHandler> GetMIDIInput() { return m_MIDIInput; }
		inline std::shared_ptr<SamplerProcessor> GetProcessor() { return m_Proc; }
		inline std::shared_ptr<SampleProgrammer> GetProgrammer() { return m_Programmer; }

	private:
		bool m_Running;
		unsigned long m_SleepTime;
		AudioProps m_Props;

		std::shared_ptr<SamplerEngine> m_Engine;
		std::shared_ptr<SamplerEventHandler> m_Handler;
		std::shared_ptr<SamplerProcessor> m_Proc;
		std::shared_ptr<SampleProgrammer> m_Programmer;
		std::shared_ptr<MIDIInputHandler> m_MIDIInput;

		void InitSampler();
		void Run();
	};
}