#include "bbpch.h"

#include "WaveOscCore.h"
#include "BackBeat/Audio/Audio.h"
namespace BackBeat {

	WaveOscCore::WaveOscCore(unsigned int sampleRate, std::shared_ptr<float[]> buffer, std::shared_ptr<OscParameters> params)
		:
		m_SampleRate(sampleRate),
		m_Buffer(buffer),
		m_WaveSize(sampleRate),
		m_Amp(1.0f),
		m_Hertz(0.0f),
		m_DutyCycle(SynthBase::WaveDutyCyleDefault),
		m_FloatPos(0.0f),
		m_Increment(1.0f),
		m_WaveType(params->wave),
		m_Wave(new float[s_BufferSize]),
		m_ModInput(std::make_unique<Modulator>(sampleRate)),
		m_Type(ModuleType::WaveOscCore),
		m_Params(params)
	{
		m_ModInput->FlushBuffer(sampleRate / Audio::Stereo, 0.0f);
	}

	WaveOscCore::~WaveOscCore()
	{
		
	}

	void WaveOscCore::Reset(unsigned int sampleRate)
	{
		m_FloatPos = 0.0f;
	}

	void WaveOscCore::Update()
	{
		auto modInput = m_ModInput->GetBuffer();
		m_Amp = m_Params->amp;
		
		float pitchShiftCents = m_Params->detune + modInput[0] * SynthBase::LFOModToPitchMax;
		float pitchShift = powf(2.0f, pitchShiftCents / Audio::CentsPerOctave);
		m_Increment = pitchShift;
		
		if (m_WaveType != m_Params->wave)
			InitWave();
	}
	
	void WaveOscCore::Render(unsigned int numSamples)
	{
		Update();

		unsigned int position = 0;
		for (unsigned int i = 0; i < numSamples * Audio::Stereo; i+= Audio::Stereo)
		{
			for (unsigned int j = 0; j < Audio::Stereo; j++)
			{
				if (m_FloatPos + (float)j >= (float)m_WaveSize)
					m_FloatPos = m_FloatPos - (float)m_WaveSize;

				position = (unsigned int)floor(m_FloatPos) + j;
				m_Buffer.get()[i + j] += (m_Wave.get()[position] * m_Amp);
			}

			m_FloatPos += m_Increment * Audio::Stereo;
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

		Audio::Bound(freq, SynthBase::CMinus1Frequency, (float)m_SampleRate / 2.0f);

		m_FloatPos = 0.0f;
		m_WaveSize = (unsigned int)(m_SampleRate / freq);
		m_DutyCycle = m_Params->dutyCycle;
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
			Wave::GetSquareWave(m_Wave, m_WaveSize, Audio::Stereo, m_DutyCycle);
			break;
		}

		}
	}

}