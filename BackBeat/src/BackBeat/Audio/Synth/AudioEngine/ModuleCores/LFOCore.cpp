#include "bbpch.h"

// TODO: Add delay

#include "LFOCore.h"
namespace BackBeat {

	LFOCore::LFOCore(UINT32 sampleRate, UINT32 bufferSize, std::shared_ptr<LFOParameters> params)
		:
		m_SampleRate(sampleRate),
		m_WaveSize((UINT32)(sampleRate * STEREO / LFO_FREQ_MIN)),
		m_Wave(new float[(UINT32)(sampleRate * STEREO / LFO_FREQ_MIN)]),
		m_Position(0),
		m_Amp(1.0f),
		m_Hertz(0.0f),
		m_WaveType(params->wave),
		m_Type(ModuleType::WaveOscCore),
		m_Params(params),
		m_Input(std::make_unique<Modulator>(bufferSize)),
		m_Output(std::make_unique<Modulator>(bufferSize))
	{

	}

	LFOCore::~LFOCore()
	{
		delete[m_WaveSize] m_Wave;
	}

	void LFOCore::Reset(UINT32 sampleRate)
	{
		m_Position = 0;
	}

	void LFOCore::Update()
	{
		m_Amp = m_Params->amp;
		if (!Audio::EqualsFloat(m_Hertz, m_Params->hertz, DELTA_CENTS_HZ) || m_WaveType != m_Params->wave) {
			InitWave();
		}
	}

	// TODO: Change to allow for modulator input
	void LFOCore::Render(UINT32 numSamples)
	{
		Update();

		// auto inputBuffer = m_Input->GetBuffer();
		auto outputBuffer = m_Output->GetBuffer();
		for (UINT32 i = 0; i < numSamples * STEREO; i++) {
			outputBuffer[i] = (m_Wave[m_Position] * m_Amp);
			m_Position = (m_Position + 1) % (m_WaveSize);
		}
	}

	void LFOCore::DoNoteOn(NoteEvent event)
	{
		m_Position = 0;
		InitWave();
	}

	void LFOCore::DoNoteOff(NoteEvent event)
	{

	}

	void LFOCore::InitWave()
	{
		m_Hertz = m_Params->hertz;
		if (m_Hertz > LFO_FREQ_MAX)
			m_Hertz = LFO_FREQ_MAX;
		else if (m_Hertz < LFO_FREQ_MIN)
			m_Hertz = LFO_FREQ_MIN;
		m_Position = 0;
		m_WaveSize = (UINT32)(m_SampleRate / m_Hertz);
		m_WaveType = m_Params->wave;

		switch (m_WaveType)
		{

		case WaveType::Sin:
		{
			Wave::GetSinWave(m_Wave, m_WaveSize, STEREO);
			break;
		}

		case WaveType::SawtoothUp:
		{
			Wave::GetSawtoothUpWave(m_Wave, m_WaveSize, STEREO);
			break;
		}

		case WaveType::SawtoothDown:
		{
			Wave::GetSawtoothDownWave(m_Wave, m_WaveSize, STEREO);
			break;
		}

		case WaveType::Triangle:
		{
			Wave::GetTriangleWave(m_Wave, m_WaveSize, STEREO);
			break;
		}

		case WaveType::Square:
		{
			Wave::GetSquareWave(m_Wave, m_WaveSize, STEREO);
			break;
		}

		}
	}
}