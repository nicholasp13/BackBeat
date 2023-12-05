#include "bbpch.h"

#include "PlayerProcessor.h"
namespace BackBeat {

	PlayerProcessor::PlayerProcessor()
		: 
		m_On(false),
		m_BufferSize(0),
		m_Output(nullptr),
		m_Track(nullptr)
	{

	}

	PlayerProcessor::~PlayerProcessor()
	{
		delete[m_BufferSize](m_Output);
	}

	void PlayerProcessor::ProcessSamples(unsigned int numSamples, unsigned int sampleRate, unsigned int numChannels)
	{
		if (!m_On)
			return;
		if (!m_Track)
			return;
		if (m_Track->GetProps().sampleRate != sampleRate) {
			BB_CORE_ERROR("SAMPLE RATE NOT SUPPORTED");
			m_On = false;
			return;
		}

		float actualSamples = (float)numSamples * (float)m_Track->GetProps().sampleRate / (float)sampleRate;
		unsigned int actualBytes = (unsigned int)floor(actualSamples * (float)m_Track->GetProps().blockAlign);
		m_Track->Render(m_Output, actualBytes);
		if (m_Track->IsDone())
			m_On = false;
		bool downSampling = m_Track->GetProps().sampleRate > sampleRate;
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

	void PlayerProcessor::PlayTrack(Track* track)
	{
		delete[m_BufferSize](m_Output);
		m_Track = track;
		m_BufferSize = track->GetProps().sampleRate;
		m_Output = new byte[m_BufferSize];
	}
}