#include "PlaybackTrack.h"

namespace Exampler {

	PlaybackTrack::PlaybackTrack()
		: m_Volume(1.0f), m_Player(nullptr) // Create in Add() function
	{

	}

	PlaybackTrack::~PlaybackTrack()
	{
		if (m_Player)
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

		ImGui::Text(m_Track->GetName().c_str());

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
		auto filePath = BackBeat::FileDialog::OpenFile("WAV Files (*.wav)\0*.wav\0");
		if (filePath.empty())
			return;

		m_Player = playerMgr->AddNewPlayer();
		auto playerID = m_Player->GetID();

		auto trackToCopy = BackBeat::TrackFactory::BuildTrack(filePath);
		if (!trackToCopy)
			return;

		BackBeat::AudioProps tempProps = trackToCopy->GetProps();

		// Temps are always floating point
		if (tempProps.bitDepth != BackBeat::Audio::FloatBitSize)
		{
			tempProps.bitDepth = BackBeat::Audio::FloatBitSize;
			tempProps.blockAlign = tempProps.numChannels * BackBeat::Audio::FloatByteSize;
			tempProps.format = BackBeat::Audio::FormatFloatingPoint;
			tempProps.byteRate = tempProps.sampleRate * tempProps.blockAlign;
		}

		m_Track = BackBeat::TrackFactory::BuildMappedTempTrack(playerID, tempProps);
		
		BackBeat::TrackFactory::CopyTrackData(trackToCopy, m_Track);

		mixer->PushProcessor(m_Player->GetProc());
		m_Player->LoadTrack(m_Track);
		m_Player->Reset();
	}

	void PlaybackTrack::Add(
		BackBeat::PlayerManager* playerMgr,
		BackBeat::RecorderManager* recorderMgr,
		BackBeat::Mixer* mixer,
		BackBeat::MIDIDeviceManager* midiDeviceManager,
		std::string filePath)
	{
		if (filePath.empty())
			return;

		m_Player = playerMgr->AddNewPlayer();
		auto playerID = m_Player->GetID();

		auto trackToCopy = BackBeat::TrackFactory::BuildTrack(filePath);
		if (!trackToCopy)
			return;

		BackBeat::AudioProps tempProps = trackToCopy->GetProps();

		// Temps are always floating point
		if (tempProps.bitDepth != BackBeat::Audio::FloatBitSize)
		{
			tempProps.bitDepth = BackBeat::Audio::FloatBitSize;
			tempProps.blockAlign = tempProps.numChannels * BackBeat::Audio::FloatByteSize;
			tempProps.format = BackBeat::Audio::FormatFloatingPoint;
			tempProps.byteRate = tempProps.sampleRate * tempProps.blockAlign;
		}

		m_Track = BackBeat::TrackFactory::BuildMappedTempTrack(playerID, tempProps);
		
		BackBeat::TrackFactory::CopyTrackData(trackToCopy, m_Track);

		mixer->PushProcessor(m_Player->GetProc());
		m_Player->LoadTrack(m_Track);
		m_Player->Reset();
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
		{
			fileNode.append_attribute("Name") = track->GetName();

			std::string trackFilePath = BackBeat::Project::GetActive()->GetConfig().tracksDirectoryPath
				+ m_Name + ".wav";
			if (BackBeat::WAVFileBuilder::BuildWAVFile(track.get(), track->GetStart(), track->GetEnd(), trackFilePath))
				fileNode.append_attribute("Path") = trackFilePath;
			else
				fileNode.append_attribute("Path") = "";
		}
		else
		{
			fileNode.append_attribute("Name");
			fileNode.append_attribute("Path");
		}
	}

	// NOTE: - node is the node being read from. This is different to WriteObject() || Might want to specify in
	//       function declaration
	void PlaybackTrack::ReadObject(pugi::xml_node* node)
	{
		m_Name = node->attribute("Name").as_string();

		m_Volume = node->child("Volume").attribute("Value").as_float();

		auto fileNode = node->child("File");
		std::string fileName = fileNode.attribute("Name").as_string();
		std::string filePath = fileNode.attribute("Path").as_string();

		if (!filePath.empty())
		{
			auto trackToCopy = BackBeat::TrackFactory::BuildTrack(filePath);
			BackBeat::TrackFactory::CopyTrackData(trackToCopy, m_Track);
			m_Track->SetName(fileName);
			m_Player->Reset();
		}
		
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