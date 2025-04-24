#include "bbpch.h"

#include "Splicer.h"
namespace BackBeat {

	Splicer::Splicer()
	{
		
	}

	Splicer::~Splicer()
	{

	}

	// This should be called by user input NOT on an update loop
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

	void Splicer::Clear()
	{
		m_Playable = false;
		m_Player.Clear();
		Audio::FlushBuffer((byte*)m_SplicedLeftChannel, SplicerMaxBufferSize * Audio::FloatByteSize);
		Audio::FlushBuffer((byte*)m_SplicedRightChannel, SplicerMaxBufferSize * Audio::FloatByteSize);
		Audio::FlushBuffer((byte*)m_InputBuffer, SplicerMaxBufferSize * Audio::Stereo * Audio::FloatByteSize);
		m_SampleOutputSize = 0;
	}

	void Splicer::SaveSample()
	{
		if (m_SampleOutputSize == 0)
			return;

		SampleBuilder::SaveSample(m_OutputProps, m_SplicedLeftChannel, m_SplicedRightChannel, m_SampleOutputSize);
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
		
		ProcessPitchShifter();

		ProcessFilters();

		ApplyGain();

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

	void Splicer::ProcessPitchShifter()
	{
		m_PitchShifter.Update(m_Params.pitchShifterParams);

		unsigned int totalInputSize = m_SampleOutputSize;
		unsigned int hopSize = m_PitchShifter.GetInputHopSize();
		unsigned int FFTSize = m_PitchShifter.GetFFTSize();

		// Process left side first
		{
			unsigned int inputIndex = 0;
			int samplesLeft = totalInputSize;
			float* inputPtr = nullptr;
			Audio::CopyInputToOutput(m_SplicedLeftChannel, m_TempBuffer, m_SampleOutputSize * Audio::FloatByteSize);

			while (samplesLeft >= (int)FFTSize)
			{
				inputPtr = m_TempBuffer + inputIndex;

				m_PitchShifter.ProcessAudioFrame(inputPtr, FFTSize);

				inputIndex += hopSize;
				samplesLeft -= hopSize;
			}

			if (samplesLeft > 0)
			{
				inputPtr = m_TempBuffer + inputIndex;
				m_PitchShifter.ProcessAudioFrame(inputPtr, samplesLeft);
			}

			auto output = m_PitchShifter.GetOutput();
			unsigned int outputSizeBytes = m_PitchShifter.GetOutputSize() * Audio::FloatByteSize;
			Audio::CopyInputToOutput(output, m_SplicedLeftChannel, outputSizeBytes);
		}

		// Process right side
		{
			m_PitchShifter.ResetOutput();

			unsigned int inputIndex = 0;
			int samplesLeft = totalInputSize;
			float* inputPtr = nullptr;
			Audio::CopyInputToOutput(m_SplicedRightChannel, m_TempBuffer, m_SampleOutputSize * Audio::FloatByteSize);

			while (samplesLeft >= (int)FFTSize)
			{
				inputPtr = m_TempBuffer + inputIndex;

				m_PitchShifter.ProcessAudioFrame(inputPtr, FFTSize);

				inputIndex += hopSize;
				samplesLeft -= hopSize;
			}

			if (samplesLeft > 0)
			{
				inputPtr = m_TempBuffer + inputIndex;

				m_PitchShifter.ProcessAudioFrame(inputPtr, samplesLeft);
			}

			auto output = m_PitchShifter.GetOutput();
			unsigned int outputSizeBytes = m_PitchShifter.GetOutputSize() * Audio::FloatByteSize;
			Audio::CopyInputToOutput(output, m_SplicedRightChannel, outputSizeBytes);
		}

		m_SampleOutputSize = m_PitchShifter.GetOutputSize();
	}
	
	void Splicer::ProcessFilters()
	{
		// Low pass filter
		if (m_Params.lowPassFilterParams.isOn)
		{
			m_LowPassFilter.Update(m_Params.lowPassFilterParams);
			m_LowPassFilter.Reset();

			// Left channel
			for (unsigned int i = 0; i < m_SampleOutputSize; i++)
			{
				m_SplicedLeftChannel[i] = m_LowPassFilter.ProcessSample(m_SplicedLeftChannel[i]);
			}

			m_LowPassFilter.Reset();

			// Right channel
			for (unsigned int i = 0; i < m_SampleOutputSize; i++)
			{
				m_SplicedRightChannel[i] = m_LowPassFilter.ProcessSample(m_SplicedRightChannel[i]);
			}
		}

		// High pass filter
		if (m_Params.highPassFilterParams.isOn)
		{
			m_HighPassFilter.Update(m_Params.highPassFilterParams);
			m_HighPassFilter.Reset();

			// Left channel
			for (unsigned int i = 0; i < m_SampleOutputSize; i++)
			{
				m_SplicedLeftChannel[i] = m_HighPassFilter.ProcessSample(m_SplicedLeftChannel[i]);
			}

			m_HighPassFilter.Reset();

			// Right channel
			for (unsigned int i = 0; i < m_SampleOutputSize; i++)
			{
				m_SplicedRightChannel[i] = m_HighPassFilter.ProcessSample(m_SplicedRightChannel[i]);
			}
		}
	}

	void Splicer::ApplyGain()
	{
		float masterGain = Audio::DecibelsToAmp(m_Params.masterGainDB);
		float leftGain = Audio::DecibelsToAmp(m_Params.leftGainDB);
		float rightGain = Audio::DecibelsToAmp(m_Params.rightGainDB);

		// Left channel
		for (unsigned int i = 0; i < m_SampleOutputSize; i++)
		{
			m_SplicedLeftChannel[i] *= masterGain * leftGain;
			if (m_SplicedLeftChannel[i] > 1.0f)
				m_SplicedLeftChannel[i] = 1.0f;
			else if (m_SplicedLeftChannel[i] < -1.0f)
				m_SplicedLeftChannel[i] = -1.0f;
		}

		// Right channel
		for (unsigned int i = 0; i < m_SampleOutputSize; i++)
		{
			m_SplicedRightChannel[i] *= masterGain * rightGain;
			if (m_SplicedRightChannel[i] > 1.0f)
				m_SplicedRightChannel[i] = 1.0f;
			else if (m_SplicedRightChannel[i] < -1.0f)
				m_SplicedRightChannel[i] = -1.0f;
		}
	}

}