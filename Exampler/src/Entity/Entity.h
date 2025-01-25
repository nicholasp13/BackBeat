#pragma once

#include "BackBeat.h"
namespace Exampler {

	enum class EntityType
	{
		none = 0,
		synth,
		sampler,
		playback,
		recording
	};

	// Externally referreed to as "Track" but not interally as it may be confused with BackBeat::Track
	class Entity : public BackBeat::Serializable
	{
	public:
		virtual void Update() {}
		virtual void OnEvent(BackBeat::Event& event) {}
		virtual void ImGuiRender() {}
		virtual void Add(
			BackBeat::PlayerManager* playerMgr,
			BackBeat::RecorderManager* recorderMgr,
			BackBeat::Mixer* mixer,
			BackBeat::MIDIDeviceManager* midiDeviceManager) {}
		virtual void Delete(
			BackBeat::PlayerManager* playerMgr,
			BackBeat::RecorderManager* recorderMgr,
			BackBeat::Mixer* mixer,
			BackBeat::MIDIDeviceManager* midiDeviceManager) {}
		virtual void Open() {}
		virtual void Close() {}
		virtual void On() {}
		virtual void Off() {}
		virtual bool IsOpen() { return false; }
		virtual bool IsOn() { return false; }

		virtual std::string GetName() { return std::string(); }
		virtual EntityType GetType() { return EntityType::none; }
		virtual std::shared_ptr<BackBeat::MappedTrack> GetMappedTrack() { return nullptr; }
		virtual void SetName(std::string name) {}
		
		inline BackBeat::UUID GetID() { return m_ID; }

	private:
		BackBeat::UUID m_ID;
	};

}