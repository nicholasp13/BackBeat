#include "bbpch.h"

#include "LFOCore.h"
namespace BackBeat {

	LFOCore::LFOCore(unsigned int sampleRate, unsigned int bufferSize, std::shared_ptr<LFOParameters> params)
		:
		m_SampleRate(sampleRate),
		m_WaveSize((unsigned int)(sampleRate * Audio::Stereo / SynthBase::LFOFrequencyMin)),
		m_Position(0),
		m_Amp(1.0f),
		m_Hertz(0.0f),
		m_Delay(SynthBase::LFODelayMin),
		m_DelayCount(0),
		m_WaveType(params->wave),
		m_Type(ModuleType::WaveOscCore),
		m_Params(params),
		m_Wave(new float[(unsigned int)(sampleRate * Audio::Stereo / SynthBase::LFOFrequencyMin)]),
		m_Input(std::make_unique<Modulator>(bufferSize)),
		m_Output(std::make_unique<Modulator>(bufferSize))
	{

	}

	LFOCore::~LFOCore()
	{
		
	}

	void LFOCore::Reset(unsigned int sampleRate)
	{
		m_Position = 0;
	}

	void LFOCore::Update()
	{
		m_Amp = m_Params->amp;
		if (!Audio::EqualsFloat(m_Hertz, m_Params->hertz, SynthBase::DeltaCentsHertz) || m_WaveType != m_Params->wave)
			InitWave();

		m_Delay = m_Params->delay;
	}

	// TODO: Change to allow for modulator input
	void LFOCore::Render(unsigned int numSamples)
	{
		Update();

		unsigned int delaySamples = unsigned int(m_Delay * m_SampleRate * Audio::Stereo);

		// auto inputBuffer = m_Input->GetBuffer();
		auto outputBuffer = m_Output->GetBuffer();

		// FLush outputBuffer
		Audio::FlushBuffer(reinterpret_cast<byte*>(outputBuffer.get()), numSamples * sizeof(float) * Audio::Stereo);

		for (unsigned int i = 0; i < numSamples * Audio::Stereo; i++) 
		{
			if (m_DelayCount > delaySamples)
			{
				outputBuffer[i] = (m_Wave[m_Position] * m_Amp);
				m_Position = (m_Position + 1) % (m_WaveSize);
			}

			if (m_DelayCount <= SynthBase::LFODelayMax * m_SampleRate * Audio::Stereo)
				m_DelayCount++;
		}
	}

	void LFOCore::DoNoteOn(NoteEvent event)
	{
		m_DelayCount = 0;
		m_Position = 0;
		InitWave();
	}

	void LFOCore::DoNoteOff(NoteEvent event)
	{

	}

	void LFOCore::InitWave()
	{
		m_Hertz = m_Params->hertz;
		if (m_Hertz > SynthBase::LFOFrequencyMax)
			m_Hertz = SynthBase::LFOFrequencyMax;
		else if (m_Hertz < SynthBase::LFOFrequencyMin)
			m_Hertz = SynthBase::LFOFrequencyMin;
		m_Position = 0;
		m_WaveSize = (unsigned int)(m_SampleRate / m_Hertz);
		m_WaveType = m_Params->wave;

		switch (m_WaveType)
		{

		case WaveType::Sin:
		{
			Wave::GetSinWave(m_Wave, m_WaveSize, Audio::Stereo);
			break;
		}

		case WaveType::SawtoothUp:
		{
			Wave::GetSawtoothUpWave(m_Wave, m_WaveSize, Audio::Stereo);
			break;
		}

		case WaveType::SawtoothDown:
		{
			Wave::GetSawtoothDownWave(m_Wave, m_WaveSize, Audio::Stereo);
			break;
		}

		case WaveType::Triangle:
		{
			Wave::GetTriangleWave(m_Wave, m_WaveSize, Audio::Stereo);
			break;
		}

		case WaveType::Square:
		{
			Wave::GetSquareWave(m_Wave, m_WaveSize, Audio::Stereo);
			break;
		}

		}
	}
}