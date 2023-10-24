#pragma once

#include "SynthModule.h"
#include "BackBeat/Audio/Synth/AudioEngine/ModuleCores/Oscillator.h"
namespace BackBeat {

	class WAVEOscillator : public SynthModule
	{
	public:
		WAVEOscillator(UINT32 sampleRate, UINT32 bufferSize, std::shared_ptr<float[]> buffer);
		~WAVEOscillator();

		virtual void Reset(UINT32 sampleRate);
		virtual void Update();
		virtual void Render(UINT32 numSamples);
		virtual void DoNoteOn(noteEvent event);
		virtual void DoNoteOff(noteEvent event);

		virtual std::shared_ptr<float[]> GetBuffer() { return m_Buffer; }

	private:
		UINT32 m_BufferSize;
		UINT32 m_SampleRate;
		UINT32 m_NumCores;
		std::shared_ptr<float[]> m_Buffer;
		std::vector< std::shared_ptr<ModuleCore> > m_Cores;

		void InitModules();
		void AddModule(std::shared_ptr<ModuleCore> module);
	};
}