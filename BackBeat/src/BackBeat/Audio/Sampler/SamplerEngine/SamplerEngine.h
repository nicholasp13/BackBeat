#pragma once

#include <queue>

#include "BackBeat/Audio/IO/RenderInfo.h"
#include "SamplerVoice.h"
namespace BackBeat {

	// TODO: Expand to 12 or 16 voices
	constexpr unsigned int SamplerMaxVoices = 10;

	class SamplerEngine
	{
	public:
		SamplerEngine(AudioProps props);
		~SamplerEngine();

		void Stop();

		virtual void Reset(unsigned int sampleRate);
		virtual void Render(std::shared_ptr<RenderInfo> info);
		virtual void ProcessMIDIEvent(MIDIEvent event);

		inline virtual std::shared_ptr<DCAParameters> GetParam(unsigned int index) { return m_Params[index]; }

		inline void ClearMIDINote(unsigned int index) { m_Voices[index]->ClearMIDINote(); }
		inline void ClearSample(unsigned int index) { m_Voices[index]->ClearSample(); }
		inline void LoopOn(unsigned int index) { m_Voices[index]->LoopOn(); }
		inline void LoopOff(unsigned int index) { m_Voices[index]->LoopOff(); }
		inline bool IsLooping(unsigned int index) { return m_Voices[index]->IsLooping(); }
		inline std::shared_ptr<SamplerCore> GetSamplerCore(unsigned int index) { return m_Voices[index]->GetCore(); }
		inline void SetNote(MIDICode code, unsigned int index) { m_Voices[index]->SetMIDINote(code); }
		inline void SetSample(Sample* newSample, unsigned int index) { m_Voices[index]->SetSample(newSample); }

	private:
		unsigned int m_NumVoices;
		unsigned int m_OutputBufferPosition;
		float m_VoiceFactor;
		AudioProps m_Props;

		std::shared_ptr<float[]> m_Buffer;
		std::vector< std::shared_ptr<DCAParameters> > m_Params;
		std::unique_ptr<SamplerVoice> m_Voices[SamplerMaxVoices];
		std::queue<MIDIEvent> m_MIDIEvents;

		void InitVoices();
		void InitParameters();
	};
}