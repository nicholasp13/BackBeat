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
	// Name of this class is subject to change
	class Entity
	{
	public:
		virtual void Update() {}
		virtual void OnEvent(BackBeat::Event& event) {}
		virtual void ImGuiRender() {}
		virtual void Delete(
			BackBeat::PlayerManager* playerMgr,
			std::shared_ptr<BackBeat::RecorderManager> recorderMgr,
			std::shared_ptr<BackBeat::Mixer> mixer,
			BackBeat::WindowsMIDIDeviceManager* midiDeviceManager
			) {}
		virtual void Open() {}
		virtual void Close() {}
		virtual void On() {}
		virtual void Off() {}
		virtual bool IsOpen() { return false; }
		virtual bool IsOn() { return false; }
		virtual std::string GetName() { return std::string(); }
		virtual EntityType GetType() { return EntityType::none; }
		virtual void SetName(std::string name) {}
		
		inline BackBeat::UUID GetID() { return m_ID; }

	private:
		BackBeat::UUID m_ID;
	};

}