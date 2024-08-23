#include "bbpch.h"

#include "PlayerProcessor.h"
namespace BackBeat {

	PlayerProcessor::PlayerProcessor()
		: 
		m_On(false),
		m_BufferSize(0),
		m_Track(nullptr)
	{

	}

	PlayerProcessor::~PlayerProcessor()
	{
		
	}

	void PlayerProcessor::ProcessSamples(unsigned int numSamples, unsigned int sampleRate, unsigned int numChannels)
	{
		if (!m_On)
			return;
		if (!m_Track)
			return;
		if (numSamples == 0)
			return;

		if (m_Track->GetProps().sampleRate != sampleRate) 
		{
			BB_CORE_ERROR("SAMPLE RATE NOT SUPPORTED");
			m_On = false;
			return;
		}

		const unsigned int bufferSize = 10000;
		AudioProps props = m_Track->GetProps();
		float actualSamples = (float)numSamples * (float)props.sampleRate / (float)sampleRate;
		unsigned int actualBytes = (unsigned int)floor(actualSamples * (float)props.blockAlign);
		
		// Flush data buffer
		for (unsigned int i = 0; i < actualBytes; i++) {
			m_Output[i] = (byte)0x00;
		}
		
		// Changes Mono to Stereo and vice versa
		if (m_Track->GetProps().numChannels != numChannels)
		{
			byte temp[bufferSize] = {};
			m_Track->Render(temp, actualBytes);
			if (numChannels == Audio::Stereo)
			{
				MonoToStereo(actualBytes, props.bitDepth, temp, m_Output);
			}
			else
			{
				StereoToMono(actualBytes, props.bitDepth, temp, m_Output);
			}
		}
		else 
		{
			m_Track->Render(m_Output, actualBytes);
		}
		
		bool downSampling = m_Track->GetProps().sampleRate > sampleRate;
		if (downSampling) 
		{
			// TODO: Implement downscaling by integer factor
			//       1. Reduce high frequency noise using a low pass filter with a cutoff frequency of
			//       2. Decimate by leaving only every Mth sample
		}
		else 
		{
			// TODO: Implement upsampling. 
			//       1. Create a new sequence of xL[n] where x[n] is the original samples seperated by L - 1 zero samples
			//       2. Fill zeros by passing through a low pass filter
		}

		if (m_Track->IsDone())
			m_On = false;
	}

	AudioProps PlayerProcessor::GetProperties()
	{
		if (!m_Track)
			return AudioProps();
		return m_Track->GetProps();
	}

	void PlayerProcessor::On()
	{
		if (!m_Track)
			return;
		m_On = true;
	}

	void PlayerProcessor::PlayTrack(std::shared_ptr<Track> track)
	{
		if (!track)
			return;
		m_Track = track;
		m_BufferSize = track->GetProps().byteRate; // NOTE: This buffer size might be too big and can make it smaller
	}

	void PlayerProcessor::ClearTrack()
	{
		m_On = false;
		m_Track = nullptr;
	}

	void PlayerProcessor::MonoToStereo(unsigned int numBytes, unsigned int bitDepth, byte* mBuffer, byte* sBuffer)
	{
		unsigned int byteDepth = bitDepth / Audio::ByteBitSize;
		unsigned int sPos = 0;
		unsigned int sIncrement = byteDepth * Audio::Stereo;

		for (unsigned int i = 0; i < numBytes; i += byteDepth) {
			for (unsigned int j = 0; j < byteDepth; j++) {
				sBuffer[sPos + j] = mBuffer[i + j];
				sBuffer[sPos + byteDepth + j ] = mBuffer[i + j];
			}
			sPos += sIncrement;
		}
	}
	
	// NOTE: Untested
	void PlayerProcessor::StereoToMono(unsigned int numBytes, unsigned int bitDepth, byte* sBuffer, byte* mBuffer)
	{
		unsigned int byteDepth = bitDepth / Audio::ByteBitSize;
		unsigned int sPos = 0;
		unsigned int sIncrement = byteDepth * Audio::Stereo;

		for (unsigned int i = 0; i < numBytes; i += byteDepth) {
			for (unsigned int j = 0; j < byteDepth; j++) {
				mBuffer[i + j] = sBuffer[sPos + j];
			}
			sPos += sIncrement;
		}
	}
}