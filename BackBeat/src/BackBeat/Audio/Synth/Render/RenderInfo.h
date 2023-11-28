#pragma once

#include <queue>

#include "BackBeat/Audio/Synth/SynthBase.h"
#include "AudioBuffer.h"
namespace BackBeat {

	class RenderInfo
	{
	public:
		RenderInfo(tWAVEFORMATEX props);
		~RenderInfo();

		midiEvent PopMIDIEvent();

		void SetSamplesToRender(UINT32 samples) { m_SamplesToRender = samples; }
		UINT32 GetSamplesToRender() { return m_SamplesToRender; }
		UINT32 GetBufferSize() { return m_BufferSize; }
		std::shared_ptr<float[]> GetBuffer() { return m_Buffer->GetBuffer(); }
		bool MIDIEventsEmpty() { return m_MIDIEvents.empty(); }
		void PushMIDIEvent(midiEvent event) { m_MIDIEvents.push(event); }

	private:
		UINT32 m_SamplesToRender;
		UINT32 m_BufferSize;

		std::shared_ptr<AudioBuffer> m_Buffer;
		std::queue<midiEvent> m_MIDIEvents;
	};
}