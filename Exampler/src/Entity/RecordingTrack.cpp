#include "RecordingTrack.h"

namespace Exampler {
	
	RecordingTrack::RecordingTrack(
		BackBeat::UUID recorderID,
		std::shared_ptr<BackBeat::Track> track,
		std::shared_ptr<BackBeat::Player> player,
		std::shared_ptr<BackBeat::RecorderManager> recorderMgr)
		: 
		m_Volume(1.0f),
		m_RecorderID(recorderID),
		m_Track(track),
		m_Player(player),
		m_RecorderMgr(recorderMgr)
	{

	}

	RecordingTrack::~RecordingTrack()
	{
		m_Player->Stop();
		std::remove(m_Track->GetFilePath().c_str());
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
		auto trackID = m_Player->GetID();
		unsigned int count = SetRecordingTrackColors();
		ImGui::PushID(trackID.ToString().c_str());
		ImGui::SeparatorText(m_Name.c_str());

		// Render canvas control buttons
		{	
			if (!(m_RecorderMgr->IsOn(m_RecorderID) && m_RecorderMgr->IsDeviceTrackOn(trackID)))
			{
				if (ImGui::Button("Record On", ImVec2(125, 20)))
					if (!m_RecorderMgr->IsRecording())
					{
						m_RecorderMgr->SetRecorderActive(m_RecorderID);
						m_RecorderMgr->SetDeviceRecorderTrack(m_Player->GetID(), m_Track);
					}
			}
			else
			{
				if (ImGui::Button("Record Off", ImVec2(125, 20)))
					if (!m_RecorderMgr->IsRecording())
					{
						m_RecorderMgr->SetRecorderInactive(m_RecorderID);
						m_RecorderMgr->ClearDeviceTrack();
					}
			} ImGui::SameLine();

			if (!m_Player->IsOn())
			{
				if (ImGui::Button("Play Recording On "))
					m_Player->On();
			}
			else
			{
				if (ImGui::Button("Play Recording Off"))
					m_Player->Off();
			} ImGui::SameLine();

			if (ImGui::Button("Clear Recording"))
			{
				if (!m_RecorderMgr->IsRecording())
				{
					m_RecorderMgr->SetDeviceRecorderTrack(m_Player->GetID(), m_Track);
					m_RecorderMgr->ResetRecorder(m_RecorderID);
					m_RecorderMgr->SetRecorderInactive(m_RecorderID);
					m_RecorderMgr->ClearDeviceTrack();
				}
			}
		}

		// Render Playback controls/info
		{
			BackBeat::TimeMinSec trackTime = m_Player->GetTime();
			BackBeat::TimeMinSec trackLength = m_Player->GetLength();

			int position = m_Player->GetPosition();
			int size = m_Player->GetSize();
			static bool wasPlaying = false;
			ImGui::Text("%d:%02d", trackTime.minutes, trackTime.seconds); ImGui::SameLine();

			// Placeholder for future implementation of a custom ImGui::Timeline widget
			if (m_Player->IsLoaded() && m_Player->GetSize() > 0)
			{
				ImGui::PushID("Seekbar");
				if (BackBeat::ImGuiWidgets::ImGuiSeekBarInt("##", &position, m_Player->GetSize(), "", ImGuiSliderFlags(0)))
				{
					if (m_Player->IsPlaying())
					{
						m_Player->Pause();
						wasPlaying = true;
					}
					m_Player->SetPosition(position);
				}
				if (ImGui::IsItemDeactivated() && wasPlaying)
				{
					m_Player->Play();
					wasPlaying = false;
				}
				ImGui::SameLine(); ImGui::Text("%d:%02d", trackLength.minutes, trackLength.seconds);
				ImGui::PopID();
			}
			else
			{
				// Renders an empty, uninteractable seek bar if no track is loaded
				ImGui::PushID("EmptySeekbar");
				int temp = 0;
				BackBeat::ImGuiWidgets::ImGuiSeekBarInt("##", &temp, 10000, "", ImGuiSliderFlags(0));
				ImGui::SameLine(); ImGui::Text("%d:%02d", trackLength.minutes, trackLength.seconds);
				ImGui::PopID();

			}
			ImGui::Spacing();

			ImGui::Text("    "); ImGui::SameLine();
			BackBeat::ImGuiWidgets::ImGuiSeekBarFloat("Volume", &m_Volume, 1.0f, "", ImGuiSliderFlags(0));
			m_Player->SetVolume(m_Volume);
		}

		ImGui::Spacing();
		ImGui::PopStyleColor(count);
		ImGui::PopID();
	}

	void RecordingTrack::Delete(
		BackBeat::PlayerManager* playerMgr,
		std::shared_ptr<BackBeat::RecorderManager> recorderMgr,
		std::shared_ptr<BackBeat::Mixer> mixer,
		BackBeat::WindowsMIDIDeviceManager* midiDeviceManager)
	{
		auto playerID = m_Player->GetID();

		m_Player->Stop();
		m_Player->ClearTrack();
		m_RecorderMgr->SetRecorderInactive(m_RecorderID);
		m_RecorderMgr->ClearDeviceTrack();

		playerMgr->Delete(playerID);
		mixer->DeleteProcessor(playerID);
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
