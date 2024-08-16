#pragma once

#include "BackBeat/Audio/MIDI/MIDIInputHandler.h"
#include "BackBeat/Audio/Instruments/Sampler/SamplerEngine/SamplerEngine.h"
#include "BackBeat/Audio/Instruments/Sampler/SamplerIO/SampleProgrammer.h"
#include "BackBeat/Audio/Instruments/Sampler/SamplerIO/SamplerEventHandler.h"
#include "BackBeat/Audio/Instruments/Sampler/SamplerIO/SamplerProcessor.h"
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
		inline UUID GetID() { return m_ID; }
		inline std::shared_ptr<SamplerEventHandler> GetEventHandler() { return m_Handler; }
		inline std::shared_ptr<MIDIInputHandler> GetMIDIInput() { return m_MIDIInput; }
		inline std::shared_ptr<SamplerProcessor> GetProcessor() { return m_Processor; }
		inline std::shared_ptr<SampleProgrammer> GetProgrammer() { return m_Programmer; }
		inline std::shared_ptr<SamplerParameters> GetParams() { return m_Params; }
		inline std::shared_ptr<SamplerEngineParameters> GetEngineParams() { return m_Params->engineParams; }

	private:
		bool m_Running;
		unsigned long m_SleepTime;
		AudioProps m_Props;
		UUID m_ID;

		std::shared_ptr<SamplerEngine> m_Engine;
		std::shared_ptr<SamplerEventHandler> m_Handler;
		std::shared_ptr<SamplerProcessor> m_Processor;
		std::shared_ptr<SampleProgrammer> m_Programmer;
		std::shared_ptr<MIDIInputHandler> m_MIDIInput;
		std::shared_ptr<SamplerParameters> m_Params;

	private:
		void InitSampler();
		void Run();
	};
}