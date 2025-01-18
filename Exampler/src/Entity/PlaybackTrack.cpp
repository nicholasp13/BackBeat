#include "PlaybackTrack.h"

namespace Exampler {

	PlaybackTrack::PlaybackTrack()
		: m_Volume(1.0f), m_Player(nullptr) // Create in Add() function
	{

	}

	PlaybackTrack::~PlaybackTrack()
	{
		m_Player->Stop();
	}

	void PlaybackTrack::Update()
	{

	}

	void PlaybackTrack::OnEvent(BackBeat::Event& event)
	{
		BackBeat::EventDispatcher dispatcher(event);
		dispatcher.Dispatch<BackBeat::KeyPressedEvent>(BIND_EVENT_FN(PlaybackTrack::OnKeyEvent));
		dispatcher.Dispatch<BackBeat::MouseButtonPressedEvent>(BIND_EVENT_FN(PlaybackTrack::OnMouseButtonEvent));
	}

	void PlaybackTrack::ImGuiRender()
	{
		ImGui::PushID(m_Player->GetID().ToString().c_str());
		unsigned int count = SetPlaybackColors();
		ImGui::SeparatorText(m_Name.c_str());

		// TODO: Wrap this text
		ImGui::Text(m_Player->GetTrackName().c_str());

		if (!m_Player->IsOn())
		{
			if (ImGui::Button("On "))
				m_Player->On();
		}
		else
		{
			if (ImGui::Button("Off"))
				m_Player->Off();
		}

		ImGui::Spacing();
			
		ImGui::Text("Volume"); ImGui::SameLine();
		BackBeat::ImGuiWidgets::ImGuiSeekBarFloat("##Volume", &m_Volume, 1.0f, "", ImGuiSliderFlags(0));
		m_Player->SetVolume(m_Volume);

		ImGui::Spacing();
		ImGui::PopStyleColor(count);
		ImGui::PopID();
	}

	void PlaybackTrack::Add(
		BackBeat::PlayerManager* playerMgr,
		BackBeat::RecorderManager* recorderMgr,
		BackBeat::Mixer* mixer,
		BackBeat::MIDIDeviceManager* midiDeviceManager)
	{
		m_Player = playerMgr->AddNewPlayer();
		mixer->PushProcessor(m_Player->GetProc());
		// m_Player->LoadTrack(BackBeat::FileDialog::OpenFile("WAV Files (*.wav)\0*.wav\0"));
		m_Player->LoadTrack(BackBeat::TrackFactory::BuildMappedTrack(
			BackBeat::FileDialog::OpenFile("WAV Files (*.wav)\0*.wav\0")));
	}

	void PlaybackTrack::Add(
		BackBeat::PlayerManager* playerMgr,
		BackBeat::RecorderManager* recorderMgr,
		BackBeat::Mixer* mixer,
		BackBeat::MIDIDeviceManager* midiDeviceManager,
		std::string filePath)
	{
		m_Player = playerMgr->AddNewPlayer();
		mixer->PushProcessor(m_Player->GetProc());
		// m_Player->LoadTrack(filePath);
		m_Player->LoadTrack(BackBeat::TrackFactory::BuildMappedTrack(filePath));

	}

	void PlaybackTrack::Delete(
		BackBeat::PlayerManager* playerMgr,
		BackBeat::RecorderManager* recorderMgr,
		BackBeat::Mixer* mixer,
		BackBeat::MIDIDeviceManager* midiDeviceManager)
	{
		auto playerID = m_Player->GetID();

		m_Player->Stop();
		m_Player->ClearTrack();

		playerMgr->Delete(playerID);
		mixer->DeleteProcessor(playerID);
	}

	// NOTE: - node is the parent of the node being written to
	void PlaybackTrack::WriteObject(pugi::xml_node* node)
	{
		auto playbackNode = node->append_child("Playback");

		playbackNode.append_attribute("Name") = m_Name;

		auto volumeNode = playbackNode.append_child("Volume");
		volumeNode.append_attribute("Value") = m_Volume;

		auto fileNode = playbackNode.append_child("File");
		auto track = m_Player->GetTrack();
		if (track)
			fileNode.append_attribute("Path") = track->GetFilePath();
		else
			fileNode.append_attribute("Path");
	}

	// NOTE: - node is the node being read from. This is different to WriteObject() || Might want to specify in
	//       function declaration
	void PlaybackTrack::ReadObject(pugi::xml_node* node)
	{
		m_Name = node->attribute("Name").as_string();

		m_Volume = node->child("Volume").attribute("Value").as_float();

		// NOTE: Track is added in MainLayer no need to read from here unless playback changes functionality
	}

	unsigned int PlaybackTrack::SetPlaybackColors()
	{
		unsigned int count = 0;

		// Misc colors
		ImGui::PushStyleColor(ImGuiCol_Separator, IM_COL32(124, 100, 110, 255)); count++;
		ImGui::PushStyleColor(ImGuiCol_PlotHistogram, IM_COL32(248, 200, 220, 255)); count++;
		ImGui::PushStyleColor(ImGuiCol_SliderGrab, IM_COL32(248, 200, 220, 255)); count++;
		ImGui::PushStyleColor(ImGuiCol_SliderGrabActive, IM_COL32(248, 200, 220, 255)); count++;

		return count;
	}

}