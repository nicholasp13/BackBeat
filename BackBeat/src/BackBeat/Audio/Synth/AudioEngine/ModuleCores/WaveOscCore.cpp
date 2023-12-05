#include "bbpch.h"

#include "WaveOscCore.h"
#include "BackBeat/Audio/Audio.h"
namespace BackBeat {

	WaveOscCore::WaveOscCore(UINT32 sampleRate, std::shared_ptr<float[]> buffer, std::shared_ptr<OscParameters> params)
		:
		m_SampleRate(sampleRate),
		m_Buffer(buffer),
		m_WaveSize(sampleRate),
		m_Wave(new float[sampleRate]),
		m_Position(0),
		m_Amp(1.0f),
		m_Hertz(0.0f),
		m_WaveType(params->wave),
		m_ModInput(std::make_unique<Modulator>(sampleRate)),
		m_Type(ModuleType::WaveOscCore),
		m_Params(params)
	{
		m_ModInput->FlushBuffer(sampleRate / STEREO, 0.0f);
	}

	WaveOscCore::~WaveOscCore()
	{
		delete[m_WaveSize] m_Wave;
	}

	void WaveOscCore::Reset(UINT32 sampleRate)
	{
		m_Position = 0;
	}

	void WaveOscCore::Update()
	{
		m_Amp = m_Params->amp;
		if (m_WaveType != m_Params->wave)
			InitWave();
	}
	
	// NOTE: FM Synthesis implementation is only roughly estimated and might not fully recreate the exact sound effect of
	//       FM Synthesis. Code subject to change.
	void WaveOscCore::Render(UINT32 numSamples)
	{
		Update();

		UINT32 position2 = (m_Position + 2) % (m_WaveSize);;
		auto modInput = m_ModInput->GetBuffer();
		int offset = 0;
		int position = m_Position;
		
		for (UINT32 i = 0; i < numSamples * STEREO; i++) {
			offset = (int)ceil(modInput[i]) * STEREO;
			position = (m_Position + offset) % m_WaveSize;
			m_Buffer[i] += (m_Wave[position] * m_Amp);
			m_Position = (m_Position + 1) % (m_WaveSize);
		}

		m_ModInput->FlushBuffer(numSamples, 0.0f);
	}

	void WaveOscCore::DoNoteOn(NoteEvent event)
	{
		m_Amp = m_Params->amp;
		m_Hertz = event.note;
		InitWave();
	}

	void WaveOscCore::DoNoteOff(NoteEvent event)
	{

	}

	void WaveOscCore::InitWave()
	{
		float freq = m_Hertz * m_Params->octave;
		if (freq > G_9_NOTE)
			freq = G_9_NOTE;
		else if (freq < C_MINUS_1_NOTE)
			freq = C_MINUS_1_NOTE;
		m_Position = 0;
		m_WaveSize = (UINT32)(m_SampleRate / freq);
		m_WaveType = m_Params->wave;

		switch(m_WaveType)
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