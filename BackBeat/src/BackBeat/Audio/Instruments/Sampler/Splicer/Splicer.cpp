#include "bbpch.h"

#include "Splicer.h"
namespace BackBeat {

	Splicer::Splicer()
		:
		m_Playable(false),
		m_SampleInputSize(0),
		m_SampleOutputSize(0),
		m_Params(SplicerParameters()),
		m_InputProps(AudioProps())
	{
		
	}

	Splicer::~Splicer()
	{

	}

	void Splicer::Update(SplicerParameters params)
	{
		if (m_SampleInputSize == 0)
			return;
		if (m_Params == params)
			return;

		m_Params = params;
		ProcessFx();

		m_Player.Load(m_SplicedLeftChannel, m_SplicedRightChannel, m_SampleOutputSize);
	}

	void Splicer::Reset()
	{
		if (m_SampleInputSize == 0)
			return;

		m_Playable = false;
		SplitInputBuffer();
		m_Playable = true;
	}

	// This takes at most half of its max buffer size of input since some fx increase the length of the output
	// @params:
	// - sampleData - buffer filled with audio data with either a single channel or left channel right channel alternating data
	// - numFrames  - number of samples / number of channels
	// - props      - audio props of sampleData
	bool Splicer::SetSampleData(float* sampleData, unsigned int numFrames, AudioProps props)
	{
		if (!sampleData)
			return false;
		if (props.numChannels > Audio::Stereo)
			return false;

		unsigned int numSamples = numFrames * props.numChannels;
		if (numSamples > SplicerMaxBufferSize)
			return false;

		m_Playable = false;
		m_SampleInputSize = numSamples;
		m_InputProps = props;

		unsigned int numBytes = numFrames * props.blockAlign;
		Audio::CopyInputToOutput(m_InputBuffer, sampleData, numBytes);

		SplitInputBuffer();

		m_Playable = true;
		return true;
	}

	// This takes at most half of its max buffer size of input since some fx increase the length of the output
	// @params:
	// - track  - Relevant track with the data to be sampled
	// - start  - start of sample in track in bytes, not block aligned
	// - end    - end of sample in track in bytes, not block aligned
	bool Splicer::SetSampleData(std::shared_ptr<Track> track, unsigned int start, unsigned int end)
	{
		if (!track)
			return false;

		AudioProps props = track->GetProps();

		if (props.numChannels > Audio::Stereo)
			return false;

		unsigned int numBytes = end - start;
		numBytes -= (numBytes % props.blockAlign);
		unsigned int numFrames = numBytes / props.blockAlign;
		unsigned int numSamples = numFrames * props.numChannels;
		if (numSamples > SplicerMaxBufferSize)
			return false;

		m_Playable = false;
		m_SampleInputSize = numSamples;
		m_InputProps = props;
		
		track->SetStart(start);
		track->SetEnd(end);
		track->SetPosition(start);
		track->SetVolume(1.0f);
		track->Read((byte*)m_InputBuffer, numBytes);

		SplitInputBuffer();

		m_Player.Load(m_SplicedLeftChannel, m_SplicedRightChannel, numFrames);

		m_Playable = true;
		return true;
	}

	void Splicer::SplitInputBuffer()
	{
		if (m_InputProps.numChannels == Audio::Mono)
		{
			for (unsigned int i = 0; i < m_SampleInputSize; i++)
			{
				m_SplicedLeftChannel[i] = m_InputBuffer[i];
				m_SplicedRightChannel[i] = m_InputBuffer[i];
			}
			m_SampleOutputSize = m_SampleInputSize;
		}
		else if (m_InputProps.numChannels == Audio::Stereo)
		{
			unsigned int leftChannelIndex = 0;
			unsigned int rightChannelIndex = 0;
			for (unsigned int i = 0; i < m_SampleInputSize; i++)
			{
				if (i % 2 == 0)
					m_SplicedLeftChannel[leftChannelIndex++] = m_InputBuffer[i];
				else
					m_SplicedRightChannel[rightChannelIndex++] = m_InputBuffer[i];
			}
			m_SampleOutputSize = m_SampleInputSize / 2;
		}
	}

	void Splicer::ProcessFx()
	{
		m_Playable = false;
		m_Player.Off();
		
		SplitInputBuffer();

		ProcessTimeStretcher();

		Pan();

		m_Playable = true;
	}

	void Splicer::ProcessTimeStretcher()
	{
		m_TimeStretcher.Update(m_Params.timeStretcherParams);

		unsigned int totalInputSize = m_SampleOutputSize;
		unsigned int inputHopSize = m_TimeStretcher.GetInputHopSize();
		unsigned int FFTSize = m_TimeStretcher.GetFFTSize();

		// Process left side first
		{
			unsigned int inputIndex = 0;
			int samplesLeft = totalInputSize;
			float* inputPtr = nullptr;
			Audio::CopyInputToOutput(m_SplicedLeftChannel, m_TempBuffer, m_SampleOutputSize * Audio::FloatByteSize);

			while (samplesLeft >= (int)FFTSize)
			{
				inputPtr = m_TempBuffer + inputIndex;
				
				m_TimeStretcher.ProcessAudioFrame(inputPtr, FFTSize);

				inputIndex += inputHopSize;
				samplesLeft -= inputHopSize;
			}  

			if (samplesLeft > 0)
			{
				inputPtr = m_TempBuffer + inputIndex;
				m_TimeStretcher.ProcessAudioFrame(inputPtr, samplesLeft);
			}

			auto output = m_TimeStretcher.GetOutput();
			unsigned int outputSizeBytes = m_TimeStretcher.GetOutputSize() * Audio::FloatByteSize;
			Audio::CopyInputToOutput(output, m_SplicedLeftChannel, outputSizeBytes);
		}

		// Process right side
		{
			m_TimeStretcher.ResetOutput();

			unsigned int inputIndex = 0;
			int samplesLeft = totalInputSize;
			float* inputPtr = nullptr;
			Audio::CopyInputToOutput(m_SplicedRightChannel, m_TempBuffer, m_SampleOutputSize * Audio::FloatByteSize);

			while (samplesLeft >= (int)FFTSize)
			{
				inputPtr = m_TempBuffer + inputIndex;

				m_TimeStretcher.ProcessAudioFrame(inputPtr, FFTSize);

				inputIndex += inputHopSize;
				samplesLeft -= inputHopSize;
			}

			if (samplesLeft > 0)
			{
				inputPtr = m_TempBuffer + inputIndex;

				m_TimeStretcher.ProcessAudioFrame(inputPtr, samplesLeft);
			}

			auto output = m_TimeStretcher.GetOutput();
			unsigned int outputSizeBytes = m_TimeStretcher.GetOutputSize() * Audio::FloatByteSize;
			Audio::CopyInputToOutput(output, m_SplicedRightChannel, outputSizeBytes);
		}

		m_SampleOutputSize = m_TimeStretcher.GetOutputSize();
	}

	void Splicer::Pan()
	{

	}

}