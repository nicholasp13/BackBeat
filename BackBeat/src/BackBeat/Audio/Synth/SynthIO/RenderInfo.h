#pragma once

#include <queue>

#include "BackBeat/Audio/Synth/SynthBase.h"
#include "BackBeat/Audio/IO/AudioBuffer.h"
namespace BackBeat {

	// TODO: Add way to manage number of midi events

	class RenderInfo
	{
	public:
		RenderInfo(AudioBuffer* buffer);
		~RenderInfo();

		MIDIEvent PopMIDIEvent();

		void SetSamplesToRender(unsigned long samples) { m_SamplesToRender = samples; }
		unsigned long GetSamplesToRender() { return m_SamplesToRender; }
		std::shared_ptr<float[]> GetBuffer() { return m_Buffer->GetBuffer(); }
		unsigned int GetBufferSize() { return m_Buffer->GetSize(); }
		bool MIDIEventsEmpty() { return m_MIDIEvents.empty(); }
		void PushMIDIEvent(MIDIEvent event) { m_MIDIEvents.push(event); }

	private:
		unsigned long m_SamplesToRender;

		AudioBuffer* m_Buffer;
		std::queue<MIDIEvent> m_MIDIEvents;
	};
}