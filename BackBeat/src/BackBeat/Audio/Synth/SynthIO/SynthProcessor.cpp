#include "bbpch.h"

#include "SynthProcessor.h"
namespace BackBeat {

	SynthProcessor::SynthProcessor(AudioProps props, std::shared_ptr<AudioEngine> engine)
		: 
		m_On(false),
		m_Output(new float[props.sampleRate]),
		m_Props(props), 
		m_Bus(props, BusDirection::Output), 
		m_Engine(engine),
		m_Info(std::make_shared<RenderInfo>(m_Bus.GetBuffer()))
	{

	}

	SynthProcessor::~SynthProcessor()
	{
		delete[m_Props.sampleRate] m_Output;
	}

	void SynthProcessor::ProcessSamples(unsigned int numSamples, unsigned int sampleRate, unsigned int numChannels)
	{
		if (!m_On)
			return;
		if (numSamples * m_Props.numChannels > m_Bus.GetBufferSize())
			return;

		m_Info->SetSamplesToRender(numSamples); // NOTE: numSamples should be scaled with the target sampleRate
		m_Engine->Render(m_Info);

		// NOTE: Synth's should always be a floating point (or a double) and this is assumed and not checked
		float* targetBuffer = reinterpret_cast<float*>(m_Output);
		auto srcBuffer = m_Bus.GetBuffer()->GetBuffer();
		if (m_Props.sampleRate == sampleRate) {
			if (numChannels == STEREO) {
				for (unsigned int i = 0; i < numSamples * m_Props.numChannels; i += m_Props.numChannels) {
					for (unsigned int j = 0; j < m_Props.numChannels; j++) {
						targetBuffer[i + j] = (float)srcBuffer[i + j];
					}
				}
			}
			else {
				unsigned int index = 0;
				for (unsigned int i = 0; i < numSamples * m_Props.numChannels; i += m_Props.numChannels) {
					targetBuffer[i] = srcBuffer[index];
					index += m_Props.numChannels;
				}
			}
		}

		bool downSampling = (m_Props.sampleRate > sampleRate);
		if (downSampling) {
			// TODO: Implement downscaling by integer factor
			//       1. Reduce high frequency noise using a low pass filter with a cutoff frequency of
			//       2. Decimate by leaving only every Mth sample
		}
		else {
			// TODO: Implement upsampling. 
			//       1. Create a new sequence of xL[n] where x[n] is the original samples seperated by L - 1 zero samples
			//       2. Fill zeros by passing through a low pass filter
		}
	}

	void SynthProcessor::Off()
	{
		m_On = false;
		Audio::FlushBuffer(m_Bus.GetBuffer()->GetBuffer(), m_Bus.GetBufferSize(), 1, 0.0f);
	}
}