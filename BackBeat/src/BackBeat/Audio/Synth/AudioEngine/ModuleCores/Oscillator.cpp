#include "bbpch.h"

#include "Oscillator.h"
#include "BackBeat/Audio/Audio.h"

namespace BackBeat {

	Oscillator::Oscillator(UINT32 sampleRate, UINT32 bufferSize, std::shared_ptr<float[]> buffer, WaveType wave)
		:
		m_SampleRate(sampleRate),
		m_BufferSize(bufferSize),
		m_Buffer(buffer),
		m_WaveSize(bufferSize),
		m_Wave(new float[bufferSize]),
		m_Position(0),
		m_Amp(1.0f),
		m_WaveType(wave),
		m_Type(ModuleType::Oscillator)
	{
		
	}

	Oscillator::~Oscillator()
	{
		// delete m_Wave;
	}

	void Oscillator::Reset(UINT32 sampleRate)
	{
		m_SampleRate = sampleRate;
		m_Position = 0;
	}

	// TODO: Implement after creating ModulationMatrix object to allow for dynamic change
	void Oscillator::Update()
	{

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
		m_Position = 0;
		InitWave(event.note, m_Amp);
	}

	void Oscillator::DoNoteOff(noteEvent event)
	{

	}

	void Oscillator::InitWave(float hertz, float amp)
	{
		m_Position = 0;
		m_WaveSize = (UINT32)(m_BufferSize / hertz);

		switch(m_WaveType)
		{

		case WaveType::Sin:
		{
			Wave::GetSinWave(m_Wave, m_WaveSize, STEREO, amp);
			break;
		}

		case WaveType::SawtoothUp:
		{
			Wave::GetSawtoothUpWave(m_Wave, m_WaveSize, STEREO, amp);
			break;
		}

		case WaveType::SawtoothDown:
		{
			Wave::GetSawtoothDownWave(m_Wave, m_WaveSize, STEREO, amp);
			break;
		}

		case WaveType::Triangle:
		{
			Wave::GetTriangleWave(m_Wave, m_WaveSize, STEREO, amp);
			break;
		}

		case WaveType::Square:
		{
			Wave::GetSquareWave(m_Wave, m_WaveSize, STEREO, amp);
			break;
		}

		}
	}

}