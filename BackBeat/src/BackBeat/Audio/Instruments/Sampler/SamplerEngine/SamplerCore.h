#pragma once

#include "BackBeat/Audio/Instruments/ModuleCore.h"
#include "BackBeat/Audio/Instruments/Sampler/Sample.h"
namespace BackBeat {

	class SamplerCore : public ModuleCore
	{
	public:
		SamplerCore(unsigned int sampleRate, std::shared_ptr<float[]> buffer);
		~SamplerCore();

		virtual void Reset(unsigned int sampleRate) override;
		virtual void Update() override;
		virtual void Render(unsigned int numSamples) override;
		virtual void DoNoteOn(NoteEvent event) override;
		virtual void DoNoteOff(NoteEvent event) override;
		
		bool IsActive();
		void SetSample(std::shared_ptr<Sample> newSample);
		std::string GetName();

		inline virtual std::shared_ptr<float[]> GetBuffer() override { return m_Output; }

		inline void LoopOn() { m_Looping = true; }
		inline void LoopOff() { m_Looping = false; }
		inline bool IsLooping() { return m_Looping; }
		inline bool IsSampleSet() { return (m_Sample != nullptr); }
		inline void ClearSample() { m_Sample = nullptr; }

	private:
		static const unsigned int s_BufferSize = 100000;

		bool m_Looping;
		unsigned int m_SampleRate;
		std::shared_ptr<Sample> m_Sample;
		float m_Input[s_BufferSize];
		std::shared_ptr<float[]> m_Output;

	};
}