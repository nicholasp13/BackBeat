#pragma once

/**
*  Soft wrapper over RenderInfo so that only MIDI events can be inputted without exposing RenderInfo
*/

#include "RenderInfo.h"
namespace BackBeat {

	class MIDIInputHandler
	{
	public:
		MIDIInputHandler(std::shared_ptr<RenderInfo> renderInfo);
		~MIDIInputHandler();

		void In(MIDIEvent event) { m_RenderInfo->PushMIDIEvent(event); }

	private:
		std::shared_ptr<RenderInfo> m_RenderInfo;
	};
}