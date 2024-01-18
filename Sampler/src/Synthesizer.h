#pragma once

// TODO:
//  Create way to save GUI settings after closing app
//  Implement ModMatrix GUI after creating ModMatrix
//  Add custom icons, objects, etc. for a more unique and cleaner look
//	Create visualizer for wavelength

#include <BackBeat.h>
#include <imgui.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>
#include <GLFW/glfw3.h>
class Synthesizer
{
public:
	Synthesizer();
	~Synthesizer();

	void Update();
	void OnEvent(BackBeat::Event& event);
	void ImGuiRender();
	void Open();
	void Close();

	inline bool IsOpen() { return m_Open; }
	inline std::shared_ptr<BackBeat::MIDIInputHandler> GetMIDIInput() { return m_Synth.GetMIDIInput(); }
	inline std::shared_ptr<BackBeat::SynthProcessor> GetSynthProc() { return m_Synth.GetProcessor(); }

private:
	bool m_Open;
	bool m_KeyboardActive;

	// Synth
	BackBeat::Synth m_Synth;
	std::shared_ptr<BackBeat::SynthParameters> m_SynthParams;
	std::shared_ptr<BackBeat::SynthEventHandler> m_SynthEventHandler;

	bool OnKeyEvent(BackBeat::KeyPressedEvent& event);
	bool OnMouseButtonEvent(BackBeat::MouseButtonPressedEvent& event);
	unsigned int SetSynthColors();
	void HelpMarker(const char* desc);
};