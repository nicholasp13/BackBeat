#pragma once

#include "BackBeat/Audio/IO/AudioProcessor.h"
namespace BackBeat {

	class SplicerPlayerProcessor : public AudioProcessor
	{
	public:
		SplicerPlayerProcessor();
		~SplicerPlayerProcessor();

		void On();
		void Play(float* data, unsigned int numFrames, AudioProps props);
		void Clear();

		inline void Reset() { m_Position = 0; }
		inline void Off() { m_On = false; }
		inline void Loop(bool loop) { m_Looping = loop; }
		inline void LoopOn() { m_Looping = true; }
		inline void LoopOff() { m_Looping = false; }
		inline bool IsLooping() { return m_Looping; }
		inline bool IsLoaded() { return m_InputBuffer; }
		inline unsigned int GetPosition() { return m_Position; }

		// AudioProcessor functions
		virtual void ProcessSamples(unsigned int numSamples, unsigned int sampleRate, unsigned int numChannels) override;
		
		inline virtual AudioProps GetProperties() override { return m_Props; }
		inline virtual AudioBus* GetBus() override { return nullptr; };
		inline virtual UUID GetID() override { return m_ID; }
		inline virtual void* GetOutputBuffer() override { return m_OutputBuffer; }
		inline virtual bool IsOn() override { return m_On; }

	private:
		static const unsigned int s_ActualOutputBufferSize = 96000; // 1 second in 48k sample rate, stereo

		bool m_On;
		bool m_Looping;
		unsigned int m_BufferSize;
		unsigned int m_Position;
		UUID m_ID;
		AudioProps m_Props;

		float* m_InputBuffer;
		float m_OutputBuffer[s_ActualOutputBufferSize] = {};
	};

}