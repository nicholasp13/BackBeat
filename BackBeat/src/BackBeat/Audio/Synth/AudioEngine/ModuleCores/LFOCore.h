#pragma once

#include "ModuleCore.h"
namespace BackBeat {

	// NOTE: Might not need to fill entire modulator array with values since LFO values vary little due to low freq
	class LFOCore : public ModuleCore
	{
	public:
		LFOCore(UINT32 sampleRate, UINT32 bufferSize, std::shared_ptr<LFOParameters> params);
		~LFOCore();

		virtual void Reset(UINT32 sampleRate);
		virtual void Update();
		virtual void Render(UINT32 numSamples);
		virtual void DoNoteOn(NoteEvent event);
		virtual void DoNoteOff(NoteEvent event);

		std::shared_ptr<float[]> GetInputBuffer() { return m_Input->GetBuffer(); }
		std::shared_ptr<float[]> GetOutputBuffer() { return m_Output->GetBuffer(); }

	private:
		UINT32 m_SampleRate;
		UINT32 m_Position;
		UINT32 m_WaveSize;
		float m_Amp;
		float m_Hertz;
		WaveType m_WaveType;
		ModuleType m_Type;

		std::shared_ptr<LFOParameters> m_Params;
		float* m_Wave;

		std::unique_ptr<Modulator> m_Input;
		std::unique_ptr<Modulator> m_Output;

		void InitWave();
	};

}