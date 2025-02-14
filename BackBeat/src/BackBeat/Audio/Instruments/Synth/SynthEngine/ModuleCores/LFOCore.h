#pragma once

#include "BackBeat/Audio/Instruments/ModuleCore.h"
namespace BackBeat {

	// NOTE: Might not need to fill entire modulator array with values since LFO values vary little due to low freq
	class LFOCore : public ModuleCore
	{
	public:
		LFOCore(unsigned int sampleRate, unsigned int bufferSize, std::shared_ptr<LFOParameters> params);
		~LFOCore();

		virtual void Reset(unsigned int sampleRate) override;
		virtual void Update() override;
		virtual void Render(unsigned int numSamples) override;
		virtual void DoNoteOn(NoteEvent event) override;
		virtual void DoNoteOff(NoteEvent event) override;

		inline std::shared_ptr<float[]> GetInputBuffer() { return m_Input->GetBuffer(); }
		inline std::shared_ptr<float[]> GetOutputBuffer() { return m_Output->GetBuffer(); }

	private:
		static const unsigned int s_BufferSize = 100000;

		unsigned int m_SampleRate;
		unsigned int m_Position;
		unsigned int m_WaveSize;
		unsigned int m_DelayCount;
		float m_Amp;
		float m_Hertz;
		float m_Delay;
		WaveType m_WaveType;
		ModuleType m_Type;

		std::shared_ptr<LFOParameters> m_Params;
		std::shared_ptr<float[]> m_Wave;

		std::unique_ptr<Modulator> m_Input;
		std::unique_ptr<Modulator> m_Output;

	private:
		void InitWave();

	};

}