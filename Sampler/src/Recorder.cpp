#include "Recorder.h"

namespace BBSampler {
	
	Recorder::Recorder()
		: 
		m_Open(false), 
		m_RecorderMgr(std::make_shared<BackBeat::RecorderManager>())
	{

	}

	Recorder::~Recorder()
	{
		m_WindowsRecorder.Stop();
	}

	void Recorder::Update()
	{

	}

	void Recorder::OnEvent(BackBeat::Event& event)
	{
		BackBeat::EventDispatcher dispatcher(event);
		dispatcher.Dispatch<BackBeat::KeyPressedEvent>(BIND_EVENT_FN(Recorder::OnKeyEvent));
		dispatcher.Dispatch<BackBeat::MouseButtonPressedEvent>(BIND_EVENT_FN(Recorder::OnMouseButtonEvent));
	}

	// NOTE: Placeholder for GUI as I will be overhaulling the Sampler GUI next
	void Recorder::ImGuiRender()
	{
		if (!m_Open)
		{
			return;
		}

		const ImGuiViewport* mainViewport = ImGui::GetMainViewport();
		float x = mainViewport->WorkPos.x;
		float y = mainViewport->WorkPos.y;
		ImGui::SetNextWindowPos(ImVec2(x, y), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize(ImVec2(600.0f, 300.0f), ImGuiCond_Once);
		// Playback flags
		ImGuiWindowFlags recorder_window_flags = 0;
		recorder_window_flags |= ImGuiWindowFlags_NoCollapse;
		recorder_window_flags |= ImGuiWindowFlags_MenuBar;
		recorder_window_flags |= ImGuiWindowFlags_NoResize;

		// unsigned int count = SetRecorderColors(); // TODO: uncomment when colors are set

		ImGui::Begin("Recorder", &m_Open, recorder_window_flags);

		// Recording manager
		{
			if (!m_RecorderMgr->IsRecording())
			{
				if (ImGui::Button("Start"))
				{
					m_RecorderMgr->Start();
				}

			}
			else
			{
				if (ImGui::Button("Pause"))
				{
					m_RecorderMgr->Stop();
				}
			}
		}

		// Synth Recorder
		{
			ImGui::PushID("SynthRecorder");
			ImGui::SeparatorText("Symth Audio");

			auto synthRecorder = m_RecorderMgr->GetRecorder(m_SynthID);
			bool recording = synthRecorder->IsRecording();

			if (!recording)
			{
				if (ImGui::Button("On"))
				{
					synthRecorder->Start();
					m_RecorderMgr->Stop();
				}

			}
			else
			{
				if (ImGui::Button("Off"))
				{
					synthRecorder->Stop();
					m_RecorderMgr->Stop();
				}
			} ImGui::SameLine();

			if (ImGui::Button("Reset"))
			{
				synthRecorder->Stop();
				synthRecorder->Reset();
			} ImGui::SameLine();

			if (ImGui::Button("Save"))
			{
				if (!recording)
				{
					std::string filePath = BackBeat::FileDialog::SaveFile("WAV Files (*.wav)\0*.wav\0");
					synthRecorder->SaveWAV(filePath);
				}
			}

			BackBeat::TimeMinSec length = synthRecorder->GetLengthMinSec();
			ImGui::Text("Time: %d:%02d", length.minutes, length.seconds);
			ImGui::NewLine();

			ImGui::Separator();
			ImGui::PopID();
		}

		// Windows Recorder
		{
			ImGui::PushID("WindowsRecorder");
			ImGui::SeparatorText("Windows Audio");

			bool recording = m_WindowsRecorder.IsRecording();

			if (!recording)
			{
				if (ImGui::Button("Start"))
				{
					m_WindowsRecorder.Start();
				}

			}
			else
			{
				if (ImGui::Button("Pause"))
				{
					m_WindowsRecorder.Stop();
				}
			} ImGui::SameLine();

			if (ImGui::Button("Reset"))
			{
				m_WindowsRecorder.Stop();
				m_WindowsRecorder.Reset();
			} ImGui::SameLine();

			if (ImGui::Button("Save"))
			{
				if (!recording)
				{
					std::string filePath = BackBeat::FileDialog::SaveFile("WAV Files (*.wav)\0*.wav\0");
					m_WindowsRecorder.SaveWAV(filePath);
				}
			}

			BackBeat::TimeMinSec length = m_WindowsRecorder.GetLengthMinSec();
			ImGui::Text("Time: %d:%02d", length.minutes, length.seconds);
			ImGui::NewLine();

			if (recording)
				ImGui::Text("Recording . . .");
			ImGui::PopID();
		}

		ImGui::End();

		// ImGui::PopStyleColor(count); // TODO: uncomment when colors are set
	}

	void Recorder::Open()
	{
		m_Open = true;
	}

	void Recorder::Close()
	{
		m_Open = false;
		m_WindowsRecorder.Stop();
	}

	bool Recorder::OnKeyEvent(BackBeat::KeyPressedEvent& event)
	{
		return false;
	}

	bool Recorder::OnMouseButtonEvent(BackBeat::MouseButtonPressedEvent& event)
	{
		return false;
	}

	unsigned int Recorder::SetRecorderColors()
	{
		return 0;
	}

}
