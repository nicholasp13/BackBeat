#pragma once

#include "Recorder.h"
namespace BackBeat {

	class AudioRecorder : public Recorder
	{
	public:
		AudioRecorder(std::string tempFilePath, AudioProps props);
		~AudioRecorder();


		virtual void On() override;
		virtual void Off() override;
		virtual void Reset() override;
		virtual void Reset(AudioProps props) override;
		virtual void Record(char* data, unsigned int numSamples) override;
		virtual bool SaveWAV(std::string filePath) override;
		virtual std::shared_ptr<Track> GetRecordingTrack() override;

		inline virtual TimeMinSec GetLengthMinSec() override { return m_Recording.GetLengthMinSecs(); }
		inline virtual RecorderType GetType() override { return RecorderType::audio; }
		inline virtual bool IsOn() override { return m_On; }

	private:
		bool m_On;
		std::thread m_Thread;
		AudioProps m_AudioProps;
		Recording m_Recording;

	};

}