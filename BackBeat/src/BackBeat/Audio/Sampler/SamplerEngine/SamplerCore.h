#pragma once

#include "BackBeat/Audio/Synth/AudioEngine/ModuleCores/ModuleCore.h"
#include "BackBeat/Audio/Sampler/Sample.h"
namespace BackBeat {

	class SamplerCore : public ModuleCore
	{
	public:
		SamplerCore(unsigned int sampleRate, std::shared_ptr<float[]> buffer);
		~SamplerCore();

		virtual void Reset(unsigned int sampleRate);
		virtual void Update();
		virtual void Render(unsigned int numSamples);
		virtual void DoNoteOn(NoteEvent event);
		virtual void DoNoteOff(NoteEvent event);
		
		bool IsActive();
		void LoopOn();
		void LoopOff();
		void SetSample(Sample* newSample);
		std::string GetName();

		inline bool IsLooping() { return m_Looping; }
		inline bool IsSampleSet() { return (m_Sample != nullptr); }
		inline void ClearSample() { m_Sample = nullptr; }
		inline virtual std::shared_ptr<float[]> GetBuffer() { return m_Output; }

	private:
		bool m_Looping;
		unsigned int m_SampleRate;
		Sample* m_Sample;
		float* m_Input;
		std::shared_ptr<float[]> m_Output;

	};
}