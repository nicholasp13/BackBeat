#pragma once

#include "Recorder.h"
namespace BackBeat {

	class AudioRecorder : public Recorder
	{
	public:
		AudioRecorder(std::string tempFilePath, AudioProps props);
		~AudioRecorder();

		virtual void Start();
		virtual void Stop();
		virtual void Reset();
		virtual void Reset(AudioProps props);
		virtual void Record(char* data, unsigned int numSamples);
		virtual bool SaveWAV(std::string filePath);

		inline virtual TimeMinSec GetLengthMinSec() { return m_Recording.GetLengthMinSecs(); }
		inline virtual RecorderType GetType() { return RecorderType::audio; }
		inline virtual bool IsRecording() { return m_IsRecording; }

	private:
		bool m_IsRecording;
		std::thread m_Thread;
		AudioProps m_AudioProps;
		Recording m_Recording;

	};

}