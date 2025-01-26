#include "RecordingTrack.h"

// TODO: Allow user to split the audio channels input into MONO entities or 1 STEREO entities

namespace Exampler {
	
	RecordingTrack::RecordingTrack()
		: 
		m_Volume(1.0f),
		m_Track(nullptr),
		m_Player(nullptr),
		m_RecorderMgr(nullptr)
	{

	}

	RecordingTrack::~RecordingTrack()
	{
		m_Player->Stop();
	}

	void RecordingTrack::Update()
	{

	}

	void RecordingTrack::OnEvent(BackBeat::Event& event)
	{
		BackBeat::EventDispatcher dispatcher(event);
		dispatcher.Dispatch<BackBeat::KeyPressedEvent>(BIND_EVENT_FN(RecordingTrack::OnKeyEvent));
		dispatcher.Dispatch<BackBeat::MouseButtonPressedEvent>(BIND_EVENT_FN(RecordingTrack::OnMouseButtonEvent));
	}

	// NOTE: Placeholder for GUI as I will be overhaulling the Sampler GUI next
	void RecordingTrack::ImGuiRender()
	{
		unsigned int count = SetRecordingTrackColors();
		ImGui::PushID(m_RecorderID.ToString().c_str());
		ImGui::SeparatorText(m_Name.c_str());

		// Render canvas control buttons
		{	
			if (!m_RecorderMgr->IsActive(m_RecorderID))
			{
				if (ImGui::Button("Record On", ImVec2(125, 20)))
					if (!m_RecorderMgr->IsRecording())
						m_RecorderMgr->SetRecorderActive(m_RecorderID);
			}
			else
			{
				if (ImGui::Button("Record Off", ImVec2(125, 20)))
					if (!m_RecorderMgr->IsRecording())
						m_RecorderMgr->SetRecorderInactive(m_RecorderID);
			}

			if (!m_Player->IsOn())
			{
				if (ImGui::Button("Play Recording On "))
					m_Player->On();
			}
			else
			{
				if (ImGui::Button("Play Recording Off"))
					m_Player->Off();
			}

			if (ImGui::Button("Clear Recording"))
				if (!m_RecorderMgr->IsRecording())
					m_RecorderMgr->ResetRecording(m_RecorderID);
		}

		// Render Playback controls/info
		{
			ImGui::Text("Volume"); ImGui::SameLine();
			BackBeat::ImGuiWidgets::ImGuiSeekBarFloat("##Volume", &m_Volume, 1.0f, "", ImGuiSliderFlags(0));
			m_Player->SetVolume(m_Volume);
		}

		ImGui::Spacing();
		ImGui::PopStyleColor(count);
		ImGui::PopID();
	}

	void RecordingTrack::Add(
		BackBeat::PlayerManager* playerMgr,
		BackBeat::RecorderManager* recorderMgr,
		BackBeat::Mixer* mixer,
		BackBeat::MIDIDeviceManager* midiDeviceManager)
	{
		m_Player = playerMgr->AddNewPlayer();
		m_RecorderMgr = recorderMgr;
		m_Track = m_RecorderMgr->AddRecordingMappedTrack(m_RecorderID, BackBeat::RecorderType::device);

		m_Player->LoadTrack(m_Track);

		mixer->PushProcessor(m_Player->GetProc());
	}

	void RecordingTrack::Delete(
		BackBeat::PlayerManager* playerMgr,
		BackBeat::RecorderManager* recorderMgr,
		BackBeat::Mixer* mixer,
		BackBeat::MIDIDeviceManager* midiDeviceManager)
	{
		auto playerID = m_Player->GetID();

		m_Player->Stop();
		m_Player->ClearTrack();
		m_RecorderMgr->SetRecorderInactive(m_RecorderID);
		m_RecorderMgr->ClearTrack(m_RecorderID);

		playerMgr->Delete(playerID);
		mixer->DeleteProcessor(playerID);
	}

	// NOTE: - node is the parent of the node being written to
	void RecordingTrack::WriteObject(pugi::xml_node* node)
	{
		auto recorderNode = node->append_child("Recorder");

		recorderNode.append_attribute("Name") = m_Name;

		// Volume
		{
			auto volumeNode = recorderNode.append_child("Volume");
			volumeNode.append_attribute("Value") = m_Volume;
		}

		// Audio track
		{
			auto trackNode = recorderNode.append_child("Track");

			std::shared_ptr<BackBeat::Track> track = m_Player->GetTrack();
			if (track)
			{
				std::string trackFilePath = BackBeat::Project::GetActive()->GetConfig().tracksDirectoryPath
					+ m_Name + ".wav";

				if (BackBeat::WAVFileBuilder::BuildWAVFile(track.get(), track->GetStart(), track->GetEnd(), trackFilePath))
					trackNode.append_attribute("FilePath") = trackFilePath;
				else
					trackNode.append_attribute("FilePath") = "";
			}
			else
				trackNode.append_attribute("FilePath") = "";
		}
	}

	// NOTE: - node is the node being read from. This is different to WriteObject() || Might want to specify in
	//       function declaration
	void RecordingTrack::ReadObject(pugi::xml_node* node)
	{
		m_Name = node->attribute("Name").value();

		// Volume
		{
			auto volumeNode = node->child("Volume");
			m_Volume = volumeNode.attribute("Value").as_float();
		}

		// Audio track
		{
			auto trackNode = node->child("Track");
			std::string trackFilePath = trackNode.attribute("FilePath").as_string();
			if (!trackFilePath.empty())
			{
				auto trackToCopy = BackBeat::TrackFactory::BuildTrack(trackFilePath);
				BackBeat::TrackFactory::CopyTrackData(trackToCopy, m_Player->GetTrack());
				m_Player->Reset();
			}
		}
	}

	unsigned int RecordingTrack::SetRecordingTrackColors()
	{
		unsigned int count = 0;

		// Misc colors
		ImGui::PushStyleColor(ImGuiCol_Separator, IM_COL32(100, 47, 49, 255)); count++;
		ImGui::PushStyleColor(ImGuiCol_PlotHistogram, IM_COL32(255, 105, 97, 255)); count++;
		ImGui::PushStyleColor(ImGuiCol_SliderGrab, IM_COL32(255, 105, 97, 255)); count++;
		ImGui::PushStyleColor(ImGuiCol_SliderGrabActive, IM_COL32(255, 105, 97, 255)); count++;

		return count;
	}

}
