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
		unsigned int m_NumMIDIDevices;
		unsigned int m_NumSynths;
		unsigned int m_NumSamplers;
		unsigned int m_NumPlayback;
		unsigned int m_NumRecorders;
		std::vector< std::string > m_DeviceNames;
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

	private:
		void RenderMenubar();
		void RenderCanvas();
		void RenderAudioVisualizer();
		void RenderMgrs();
		void RenderEntities();
		void RenderEntityMenubar(unsigned int index);
		void RenderPopups();

		void AddSynth();
		void AddSampler();
		void AddPlaybackTrack();
		void AddRecordingTrack();

		void DeleteEntity();

		bool OnKeyEvent(BackBeat::KeyPressedEvent& event);
		bool OnMouseButtonEvent(BackBeat::MouseButtonPressedEvent& event);

		unsigned int SetCanvasColors();
		unsigned int SetMainColors();
	};

}