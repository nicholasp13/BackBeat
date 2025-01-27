#pragma once

#include "BackBeat/Audio/IO/AudioSink.h"
#include "Recorder.h"
namespace BackBeat {

	class AudioRecorder : public Recorder
	{
	public:
		AudioRecorder();
		~AudioRecorder();

		virtual void Start() override;
		virtual void Stop() override;
		virtual void Reset() override;
		virtual void Reset(AudioProps props) override;
		virtual bool SaveWAV(std::string filePath) override;
		virtual std::shared_ptr<MappedTrack> GetRecordingTrack() override;

		void Init(AudioProps props, AudioSink* sink);

		inline virtual void ClearTrack() override { m_Recording->ClearTrack(); }
		inline virtual bool IsRecording() { return m_IsRecording; }
		inline virtual AudioProps GetProps() override { return m_AudioProps; }
		inline virtual TimeMinSec GetLengthMinSec() override { return m_Recording->GetLengthMinSecs(); }
		inline virtual RecorderType GetType() override { return RecorderType::audio; }
		inline virtual UUID GetID() override { return m_ID; }
		inline virtual void SetRecording(std::shared_ptr<Recording> recording) override { m_Recording = recording; }


	private:
		static const unsigned int s_BufferSize = 20000; // About 1/5 of a second of samples at 48k sample rate

		bool m_IsRecording;
		bool m_Init;
		unsigned int m_BufferSize;
		float m_Buffer[s_BufferSize] = {};
		UUID m_ID;
		AudioProps m_AudioProps;
		std::shared_ptr<Recording> m_Recording;
		AudioSink* m_Sink;

	private:
		void Record();
	};

}