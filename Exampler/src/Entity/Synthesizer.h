#pragma once

#include <BackBeat.h>
#include <imgui.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>
#include <GLFW/glfw3.h>
#include <imgui-knobs.h>

#include "Entity.h"
namespace Exampler {

	class Synthesizer : public Entity
	{
	public:
		Synthesizer();
		~Synthesizer();

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

		inline virtual void Open() override { m_Open = true; };
		inline virtual void Close() override { m_Open = false; };
		inline virtual void On() override { m_Synth.Start(); }
		inline virtual void Off() override { m_Synth.Stop(); }
		inline virtual bool IsOpen() override { return m_Open; }
		inline virtual std::string GetName() override { return m_Name; }
		inline virtual EntityType GetType() override { return EntityType::synth; }
		inline virtual std::shared_ptr<BackBeat::MappedTrack> GetMappedTrack() override { return m_RecordingMappedTrack; }
		inline virtual void SetName(std::string name) override { m_Name = name; }

		// BackBeat::Serializable functions
		virtual void WriteObject(pugi::xml_node* node) override;
		virtual void ReadObject(pugi::xml_node* node) override;

		// Synthesizer functions
		inline std::shared_ptr<BackBeat::MIDIInputHandler> GetMIDIInput() { return m_Synth.GetMIDIInput(); }
		inline std::shared_ptr<BackBeat::SynthProcessor> GetSynthProc() { return m_Synth.GetProcessor(); }
		inline void SetRecordingPlayer(std::shared_ptr<BackBeat::Player> player) { m_RecordingPlayer = player; }

	private:
		const float m_Width = 800.0f;
		const float m_Height = 540.0f;

		bool m_Open;
		bool m_KeyboardActive;
		std::string m_Name;
		// Synthesizer params
		int m_NoteVelocity;
		float m_Pan;
		float m_TrackVolume;
		int m_LFOWave,
			m_OscWave1,
			m_OscWave2,
			m_OscWave3,
			m_OscWave4;
		int m_Octave1,
			m_Octave2,
			m_Octave3,
			m_Octave4;
		int m_PWM1,
			m_PWM2,
			m_PWM3,
			m_PWM4;
		// BackBeat objects
		BackBeat::Synth m_Synth;
		std::shared_ptr<BackBeat::SynthParameters> m_SynthParams;
		std::shared_ptr<BackBeat::SynthEventHandler> m_SynthEventHandler;
		std::shared_ptr<BackBeat::Player> m_RecordingPlayer;
		BackBeat::RecorderManager* m_RecorderMgr;
		std::shared_ptr<BackBeat::MappedTrack> m_RecordingMappedTrack;

	private:
		void RenderCanvasEntity();
		void RenderMenubar();
		void RenderGeneralControls();
		void RenderVolumePanControls();
		void RenderLFO();
		void RenderFilters();
		void RenderEGs();
		void RenderNoiseGenerator();
		void RenderOscs();
		void RenderOsc(const char* label, int* octave, std::shared_ptr<BackBeat::OscParameters> params,
			int* waveIndex, int* PWMIndex);

		unsigned int SetCanvasColors();
		unsigned int SetEntityColors();

		inline bool OnKeyEvent(BackBeat::KeyPressedEvent& event) { return false; }
		inline bool OnMouseButtonEvent(BackBeat::MouseButtonPressedEvent& event) { return false; }
	};

}