#pragma once

#include <BackBeat.h>
#include <imgui.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>
#include <GLFW/glfw3.h>

#include "Entity.h"
namespace Exampler {

	constexpr auto CreatingSampleID = "CreatingSample";
	constexpr auto ProgrammingNoteID = "ProgrammingNote";

	class Sampler : public Entity
	{
	public:
		Sampler();
		~Sampler();

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

		inline virtual void Open() override { m_Open = true; }
		inline virtual void Close() override { m_Open = false; }
		inline virtual void On() override { m_Sampler.Start(); };
		inline virtual void Off() override { m_Sampler.Stop(); };
		inline virtual bool IsOpen() override { return m_Open; }
		inline virtual std::string GetName() override { return m_Name; }
		inline virtual EntityType GetType() override { return EntityType::sampler; }
		inline virtual std::shared_ptr<BackBeat::MappedTrack> GetMappedTrack() override { return m_RecordingMappedTrack; }
		inline virtual void SetName(std::string name) override { m_Name = name; }

		// BackBeat::Serializable functions
		virtual void WriteObject(pugi::xml_node* node) override;
		virtual void ReadObject(pugi::xml_node* node) override;

		// Sampler functions
		inline std::shared_ptr<BackBeat::MIDIInputHandler> GetMIDIInput() { return m_Sampler.GetMIDIInput(); }
		inline std::shared_ptr<BackBeat::PlayerProcessor> GetTrackProcessor() { return m_TrackPlayer.GetProc(); }
		inline std::shared_ptr<BackBeat::SamplerProcessor> GetSamplerProcessor() { return m_Sampler.GetProcessor(); }
		inline void SetRecordingPlayer(std::shared_ptr<BackBeat::Player> player) { m_RecordingPlayer = player; }

	private:
		bool m_Open;
		bool m_KeyboardActive;
		bool m_CreatingSample;
		bool m_ProgrammingNote;
		unsigned int m_DevicesOpen;
		unsigned int m_NumPads;
		unsigned int m_PadToProgram; // This is the display Pad # not the actual index number, actual index is this - 1
		float m_TrackVolume;
		std::string m_Name;

		// BackBeat objects
		BackBeat::Player m_TrackPlayer;
		BackBeat::Sampler m_Sampler;
		std::shared_ptr<BackBeat::Player> m_RecordingPlayer;
		BackBeat::RecorderManager* m_RecorderMgr;
		std::shared_ptr<BackBeat::MappedTrack> m_RecordingMappedTrack;

	private:
		void RenderCanvasEntity();
		void RenderMenuBar();
		void RenderSamplerPads();
		void RenderSampleCreator();
		void RenderNoteProgrammer();
		unsigned int SetSamplerColors();

		inline bool OnKeyEvent(BackBeat::KeyPressedEvent& event) { return false; }
		inline bool OnMouseButtonEvent(BackBeat::MouseButtonPressedEvent& event) { return false; }
	};
}