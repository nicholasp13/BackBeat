#pragma once

#include <BackBeat.h>
#include <imgui.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>
#include <GLFW/glfw3.h>

constexpr auto CreatingSampleID = "CreatingSample";
constexpr auto ProgrammingNoteID = "ProgrammingNote";

class SamplerController
{
public: 
	SamplerController();
	~SamplerController();

	void Update();
	void OnEvent(BackBeat::Event& event);
	void ImGuiRender();
	void Open();
	void Close();

	inline bool IsOpen() { return m_Open; }
	inline std::shared_ptr<BackBeat::PlayerProcessor> GetTrackGetProcessor() { return m_TrackPlayer.GetProc(); }
	inline std::shared_ptr<BackBeat::SamplerProcessor> GetSamplerGetProcessor() { return m_Sampler.GetProcessor(); }

private:
	bool m_Open;
	bool m_KeyboardActive;
	bool m_CreatingSample;
	bool m_ProgrammingNote;
	unsigned int m_DevicesOpen;
	unsigned int m_NumMIDIDevices;
	unsigned int m_NumPads;
	unsigned int m_PadToProgram; // This is the display Pad # not the actual index number, actual index is this - 1
	std::vector<std::string> m_DeviceNames;
	
	BackBeat::Player m_TrackPlayer;
	BackBeat::Sampler m_Sampler;
	BackBeat::WindowsMIDIDeviceManager m_MIDIDeviceManager; // Not implemented right now just in case it conflicts with Synth

	bool OnKeyEvent(BackBeat::KeyPressedEvent& event);
	bool OnMouseButtonEvent(BackBeat::MouseButtonPressedEvent& event);
	void RenderMenuBar();
	void RenderSamplerPads();
	void RenderSampleCreator();
	void RenderNoteProgrammer();
	unsigned int SetSamplerColors();
};