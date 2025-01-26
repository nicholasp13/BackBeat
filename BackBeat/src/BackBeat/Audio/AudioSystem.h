#pragma once

#include "BackBeat/Audio/Renderer/Renderer.h"
#include "MIDI/MIDIDeviceManager.h"
#include "BackBeat/Audio/PlayBack/PlayerManager.h"
#include "BackBeat/Audio/Recorder/RecorderManager.h"
#include "BackBeat/Audio/Helpers/AudioThread.h"
#include "BackBeat/Audio/Visualizer/Visualizer.h"
#include "BackBeat/File/FileMapper.h"
#include "AudioEngine.h"
namespace BackBeat {

	class AudioSystem
	{
	public:
		virtual void Init(std::string appName) {}
		virtual void Start() {}
		virtual void Stop() {}
		virtual AudioProps GetProps() { return AudioProps(); }
		virtual FileMapper* GetFileMapper() { return nullptr; }
		virtual Mixer* GetMixer() { return nullptr; }
		virtual Renderer* GetRenderer() { return nullptr; }
		virtual PlayerManager* GetPlayerManager() { return nullptr; }
		virtual RecorderManager* GetRecorderManager() { return nullptr; }
		virtual MIDIDeviceManager* GetMIDIDeviceManager() { return nullptr; }
		virtual Visualizer* GetVisualizer() { return nullptr; }
	};
}