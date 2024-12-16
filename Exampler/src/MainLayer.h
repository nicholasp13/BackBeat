#pragma once

#include <BackBeat.h>
#include <imgui.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>
#include <GLFW/glfw3.h>

#include "Entity/Synthesizer.h"
#include "Entity/Sampler.h"
#include "Entity/RecordingTrack.h"
#include "Entity/PlaybackTrack.h"
#include "Entity/DummyEntity.h"
namespace Exampler {

	// Arbitrarily chosen but should be decided based on RAM size/ memory usage
	constexpr unsigned int       // Memory:
		MaxSynths = 2,           // ~90 mb
		MaxSamplers = 1,         // ~10 mb
		MaxPlayback = 8,         //  ~0 mb
		MaxRecordingDevices = 4; //  ~2 mb

	class MainLayer : public BackBeat::Layer
	{
	public:
		MainLayer(BackBeat::Window* window, BackBeat::AudioSystem* audio);
		~MainLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate() override;
		virtual void OnEvent(BackBeat::Event& event) override;
		virtual void OnImGuiRender() override;

	private:
		bool m_NewPopupOpen;
		bool m_SaveAsPopupOpen;
		unsigned int m_NumMIDIDevices;
		unsigned int m_NumSynths;
		unsigned int m_NumSamplers;
		unsigned int m_NumPlayback;
		unsigned int m_NumRecorders;
		std::string m_AppFileDir;
		std::vector< std::string > m_DeviceNames;
		std::vector< std::string > m_ProjectNames;
		std::vector< std::shared_ptr<Entity> > m_Entities;
		std::shared_ptr<Entity> m_EtyToRename;
		std::shared_ptr<Entity> m_EtyToDelete;

		// BackBeat members
		BackBeat::Window* m_Window;
		BackBeat::AudioSystem* m_Audio;
		BackBeat::PlayerManager* m_PlayerMgr;
		BackBeat::RecorderManager* m_RecorderMgr;
		BackBeat::Renderer* m_AudioRenderer;
		BackBeat::MIDIDeviceManager* m_MIDIDeviceManager;
		BackBeat::Visualizer* m_Visualizer;
		BackBeat::FileManager m_FileMgr;
		std::shared_ptr<BackBeat::Project> m_ActiveProject;

		enum class AppState
		{
			Start = 0, Play = 1
		};

		AppState m_State;

	private:
		// Rendering functions
		void RenderStartup();
		void RenderMenubar();
		void RenderCanvas();
		void RenderAudioVisualizer();
		void RenderMgrs();
		void RenderEntities();
		void RenderEntityMenubar(unsigned int index);
		void RenderPopups();

		// Entity functions
		void AddSynth();
		void AddSampler();
		void AddPlaybackTrack();
		void AddRecordingTrack();
		void DeleteEntity();

		// Project functions
		bool LoadProject(std::string project);
		void NewProject();
		void SaveProject();
		void Serialize(std::string filePath);
		void Deserialize(std::string filePath);

		// Event functions
		bool OnKeyEvent(BackBeat::KeyPressedEvent& event);
		bool OnMouseButtonEvent(BackBeat::MouseButtonPressedEvent& event);

		// Color functions
		unsigned int SetCanvasColors();
		unsigned int SetMainColors();

	};

}