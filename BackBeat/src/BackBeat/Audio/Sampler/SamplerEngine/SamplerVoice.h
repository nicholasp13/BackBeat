#pragma once

#include "SamplerModule.h"
#include "BackBeat/Audio/MIDI/MIDICodes.h"
#include "BackBeat/Audio/Synth/AudioEngine/SynthModules/DCA.h"
namespace BackBeat {

	class SamplerVoice
	{
	public:
		SamplerVoice(unsigned int bufferSize, std::shared_ptr<float[]> outputBuffer, std::shared_ptr<DCAParameters> params);
		~SamplerVoice();

		virtual void Reset(unsigned int sampleRate);
		virtual void Update();
		virtual void Render(unsigned int numSamples);
		virtual void ProcessMIDIEvent(MIDIEvent event);

		inline void ClearMIDINote() { m_MIDINote = MIDI::NoteOff; }
		inline void ClearSample() { m_SamplerModule->GetCore()->ClearSample(); }
		inline bool IsActive() { return m_SamplerModule->GetCore()->IsActive(); }
		inline void SetMIDINote(MIDICode newNote) { m_MIDINote = newNote; }
		inline void SetSample(Sample* newSample) { m_SamplerModule->GetCore()->SetSample(newSample); }
		inline MIDICode GetMIDINote() { return m_MIDINote; }
		inline std::string GetSampleName() { return m_SamplerModule->GetCore()->GetName(); }
		inline std::shared_ptr<SamplerCore> GetCore() { return m_SamplerModule->GetCore(); }

	private:
		MIDICode m_MIDINote;
		unsigned int m_BufferSize;
		std::shared_ptr<float[]> m_InputBuffer;
		std::shared_ptr<float[]> m_OutputBuffer;
		std::shared_ptr<DCAParameters> m_Params;
		std::unique_ptr<SamplerModule> m_SamplerModule;
		std::unique_ptr<DCA> m_DCA;

		virtual void DoNoteOn(NoteEvent event);
		virtual void DoNoteOff(NoteEvent event);
		void InitModules();
	};

}