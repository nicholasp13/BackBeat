#pragma once

#include "AudioRecorder.h"
#include "BackBeat/Core/UUID.h"
namespace BackBeat {

	namespace RecorderBuilder
	{
		std::shared_ptr<AudioRecorder> BuildRecorder(UUID id, AudioProps props);
	}

}