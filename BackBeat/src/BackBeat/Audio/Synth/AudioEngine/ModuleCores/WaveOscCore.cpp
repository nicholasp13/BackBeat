#include "bbpch.h"

#include "WaveOscCore.h"
#include "BackBeat/Audio/Audio.h"
namespace BackBeat {

	WaveOscCore::WaveOscCore(unsigned int sampleRate, std::shared_ptr<float[]> buffer, std::shared_ptr<OscParameters> params)
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
		m_ModInput->FlushBuffer(sampleRate / Audio::Stereo, 0.0f);
	}

	WaveOscCore::~WaveOscCore()
	{
		delete[m_WaveSize] m_Wave;
	}

	void WaveOscCore::Reset(unsigned int sampleRate)
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
	void WaveOscCore::Render(unsigned int numSamples)
	{
		Update();

		unsigned int position2 = (m_Position + 2) % (m_WaveSize);;
		auto modInput = m_ModInput->GetBuffer();
		int offset = 0;
		int position = m_Position;
		
		for (unsigned int i = 0; i < numSamples * Audio::Stereo; i++) {
			offset = (int)ceil(modInput[i]) * Audio::Stereo;
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
		if (freq > SynthBase::G_9)
			freq = SynthBase::G_9;
		else if (freq < SynthBase::C_Minus1)
			freq = SynthBase::C_Minus1;
		m_Position = 0;
		m_WaveSize = (unsigned int)(m_SampleRate / freq);
		m_WaveType = m_Params->wave;

		switch(m_WaveType)
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