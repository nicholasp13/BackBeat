#pragma once

#include "BackBeat/Audio/IO/AudioSink.h"
namespace BackBeat {

	class Visualizer
	{
	public:
		Visualizer();
		~Visualizer();

		void Init(AudioProps props, AudioSink* sink, unsigned int updateBufferSize);
		void Update();
		void On();
		void Off();
		// Note: Should call Update() before either GetBuffer() calls
		float* GetChannelBuffer(unsigned int index);
		
		inline bool IsOn() { return m_On; }
		inline AudioProps GetProps() { return m_Props; }
		inline unsigned int GetNumChannels() { return m_Props.numChannels; }
		// Note: Should call Update() before either GetBuffer()
		inline float* GetBuffer() { return m_OutputBuffer.get(); }
		inline unsigned int GetBufferSize() { return s_ArraySize; }
		inline unsigned int GetChannelBufferSize() { return m_ChannelBufferSize; }

	private:
		const unsigned int s_ArraySize = 48000;

		bool m_On;
		unsigned int m_UpdateBufferSize;
		unsigned int m_ChannelBufferSize;
		unsigned int m_ChannelUpdateSize;
		AudioProps m_Props;
		AudioSink* m_Sink;
		std::shared_ptr<float[]> m_InputBuffer;
		std::shared_ptr<float[]> m_OutputBuffer;
		std::vector< std::shared_ptr<float[]>> m_ChannelBuffers;

	};

}