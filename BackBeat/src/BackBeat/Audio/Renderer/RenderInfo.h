#pragma once

#include <queue>

#include "BackBeat/Audio/Instruments/Synth/SynthBase.h"
#include "BackBeat/Audio/IO/AudioBuffer.h"
namespace BackBeat {

	class RenderInfo
	{
	public:
		RenderInfo(AudioBuffer* buffer);
		~RenderInfo();

		MIDIEvent PopMIDIEvent();

		inline void SetSamplesToRender(unsigned long samples) { m_SamplesToRender = samples; }
		inline unsigned long GetSamplesToRender() { return m_SamplesToRender; }
		inline std::shared_ptr<float[]> GetBuffer() { return m_Buffer->GetBuffer(); }
		inline unsigned int GetBufferSize() { return m_Buffer->GetSize(); }
		inline bool MIDIEventsEmpty() { return m_MIDIEvents.empty(); }
		inline void PushMIDIEvent(MIDIEvent event) { m_MIDIEvents.push(event); }

	private:
		unsigned long m_SamplesToRender;

		AudioBuffer* m_Buffer;
		std::queue<MIDIEvent> m_MIDIEvents;
	};
}