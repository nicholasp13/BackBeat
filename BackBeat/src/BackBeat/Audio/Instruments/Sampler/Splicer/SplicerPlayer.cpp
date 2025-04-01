#include "bbpch.h"

#include "SplicerPlayer.h"
namespace BackBeat {

	SplicerPlayer::SplicerPlayer()
		:
		m_TotalFrames(0),
		m_Props(Audio::GetDefaultProps()),
		m_Proc(std::make_shared<SplicerPlayerProcessor>())
	{

	}

	SplicerPlayer::~SplicerPlayer()
	{
		m_Proc->Off();
	}

	void SplicerPlayer::Start()
	{
		if (!m_Proc->IsLoaded())
			return;

		m_Proc->Reset();
		m_Proc->On();
	}

	void SplicerPlayer::Stop()
	{
		if (!m_Proc->IsLoaded())
			return;

		m_Proc->Off();
		m_Proc->Reset();
	}

	void SplicerPlayer::Load(float* leftChannel, float* rightChannel, unsigned int numFrames)
	{
		if (!leftChannel)
			return;
		if (!rightChannel)
			return;

		unsigned int totalSamples = numFrames * Audio::Stereo;
		// NOTE: SplicerPlayerProcessor is in charge of deleting this buffer
		float* buffer = new float[totalSamples];

		unsigned int leftIndex = 0;
		unsigned int rightIndex = 0;
		for (unsigned int i = 0; i < totalSamples; i++)
		{
			if (i % 2 == 0)
				buffer[i] = leftChannel[leftIndex++];
			else
				buffer[i] = rightChannel[rightIndex++];
		}

		m_Proc->Play(buffer, numFrames, m_Props);
		m_TotalFrames = numFrames;
	}

}