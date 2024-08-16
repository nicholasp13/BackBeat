#pragma once

#include "BackBeat/Audio/Renderer/Renderer.h"
#include "MIDI/MIDIDeviceManager.h"
#include "BackBeat/Audio/PlayBack/PlayerManager.h"
#include "BackBeat/Audio/Recorder/RecorderManager.h"
namespace BackBeat {

	class AudioSystem
	{
	public:
		virtual void Init(std::string projectName) {}
		virtual void Start() {}
		virtual void Stop() {}
		virtual AudioProps GetProps() { return AudioProps(); }
		virtual Mixer* GetMixer() { return nullptr; }
		virtual Renderer* GetRenderer() { return nullptr; }
		virtual PlayerManager* GetPlayerManager() { return nullptr; }
		virtual RecorderManager* GetRecorderManager() { return nullptr; }
		virtual MIDIDeviceManager* GetMIDIDeviceManager() { return nullptr; }
		virtual void AddSampler() {}
		virtual void AddSynth() {}
		virtual void AddPlayer() {}


	};
}