#include "bbpch.h"

#include "Oscillator.h"
#include "BackBeat/Audio/Audio.h"

namespace BackBeat {

	Oscillator::Oscillator(UINT32 sampleRate, std::shared_ptr<float[]> buffer, std::shared_ptr<OscParameters> params)
		:
		m_SampleRate(sampleRate),
		m_Buffer(buffer),
		m_WaveSize(sampleRate),
		m_Wave(new float[sampleRate]),
		m_Position(0),
		m_Amp(1.0f),
		m_Hertz(0.0f),
		m_WaveType(params->wave),
		m_Type(ModuleType::Oscillator),
		m_Params(params)
	{
		
	}

	Oscillator::~Oscillator()
	{

	}

	void Oscillator::Reset(UINT32 sampleRate)
	{
		m_Position = 0;
	}

	// TODO: Implement after creating ModulationMatrix object to allow for dynamic change
	void Oscillator::Update()
	{
		if (m_WaveType != m_Params->wave || m_Amp != m_Params->amp)
			InitWave();
	}

	void Oscillator::Render(UINT32 numSamples)
	{
		Update();

		for (UINT32 i = 0; i < numSamples * STEREO; i++) {
			m_Buffer[i] = m_Wave[m_Position];
			m_Position = (m_Position + 1) % (m_WaveSize);
		}
	}

	void Oscillator::DoNoteOn(noteEvent event)
	{
		m_Amp = m_Params->amp;
		m_Hertz = event.note;
		InitWave();
	}

	void Oscillator::DoNoteOff(noteEvent event)
	{

	}

	void Oscillator::InitWave()
	{
		m_Position = 0;
		m_WaveSize = (UINT32)(m_SampleRate / m_Hertz);
		m_WaveType = m_Params->wave;
		m_Amp = m_Params->amp;

		switch(m_WaveType)
		{

		case WaveType::Sin:
		{
			Wave::GetSinWave(m_Wave, m_WaveSize, STEREO, m_Amp);
			break;
		}

		case WaveType::SawtoothUp:
		{
			Wave::GetSawtoothUpWave(m_Wave, m_WaveSize, STEREO, m_Amp);
			break;
		}

		case WaveType::SawtoothDown:
		{
			Wave::GetSawtoothDownWave(m_Wave, m_WaveSize, STEREO, m_Amp);
			break;
		}

		case WaveType::Triangle:
		{
			Wave::GetTriangleWave(m_Wave, m_WaveSize, STEREO, m_Amp);
			break;
		}

		case WaveType::Square:
		{
			Wave::GetSquareWave(m_Wave, m_WaveSize, STEREO, m_Amp);
			break;
		}

		}
	}

}