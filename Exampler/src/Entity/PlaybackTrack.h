#pragma once

#include <BackBeat.h>
#include <imgui.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>
#include <GLFW/glfw3.h>

#include "Entity.h"
namespace Exampler {

	class PlaybackTrack : public Entity
	{
	public:
		PlaybackTrack();
		~PlaybackTrack();

		// Entity functions
		virtual void Update() override;
		virtual void OnEvent(BackBeat::Event& event) override;
		virtual void ImGuiRender() override;
		virtual void Add(
			BackBeat::PlayerManager* playerMgr,
			BackBeat::RecorderManager* recorderMgr,
			BackBeat::Mixer* mixer,
			BackBeat::MIDIDeviceManager* midiDeviceManager) override;
		virtual void Delete(
			BackBeat::PlayerManager* playerMgr,
			BackBeat::RecorderManager* recorderMgr,
			BackBeat::Mixer* mixer,
			BackBeat::MIDIDeviceManager* midiDeviceManager) override;

		inline virtual void Open() override { };
		inline virtual void Close() override { };
		inline virtual void On() override { m_Player->Start(); }
		inline virtual void Off() override { m_Player->Stop(); }
		inline virtual bool IsOpen() override { return false; }
		inline virtual std::string GetName() override { return m_Name; }
		inline virtual EntityType GetType() override { return EntityType::playback; }
		inline virtual void SetName(std::string name) override { m_Name = name; }

		// BackBeat::Serializable functions
		virtual void WriteObject(pugi::xml_node* node) override;
		virtual void ReadObject(pugi::xml_node* node) override;

		// Playback functions
		inline std::shared_ptr<BackBeat::PlayerProcessor> GetProc() { return m_Player->GetProc(); }
		void Add(
			BackBeat::PlayerManager* playerMgr,
			BackBeat::RecorderManager* recorderMgr,
			BackBeat::Mixer* mixer,
			BackBeat::MIDIDeviceManager* midiDeviceManager,
			std::string filePath);

	private:
		float m_Volume;
		std::string m_Name;
		std::shared_ptr<BackBeat::Player> m_Player;

	private:
		unsigned int SetPlaybackColors();

		inline bool OnKeyEvent(BackBeat::KeyPressedEvent& event) { return false; }
		inline bool OnMouseButtonEvent(BackBeat::MouseButtonPressedEvent& event) { return false; }

	};

}