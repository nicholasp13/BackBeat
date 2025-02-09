#include "bbpch.h"

#include "TPTLPLadderFilterCore.h"
namespace BackBeat {

	TPTLPLadderFilterCore::TPTLPLadderFilterCore(unsigned int sampleRate)
		:
		m_SampleRate(sampleRate),
		m_Gain(1.0f),
		m_CutoffFreq(SynthBase::FilterCutoffMax),
		m_Type(ModuleType::LowPassFilter),
		m_G(0.0f),
		m_S(0.0f)
	{

	}

	TPTLPLadderFilterCore::~TPTLPLadderFilterCore()
	{

	}

	void TPTLPLadderFilterCore::Reset(unsigned int sampleRate)
	{

	}

	// NOTE: This will be called by module/owner and not by Render()
	void TPTLPLadderFilterCore::Update()
	{
		float g = tan(2 * SynthBase::Pi * m_CutoffFreq / (float)m_SampleRate * m_Gain / 2);
		m_G = g / (1 + g);
	}

	// NOTE: Unlike a singular filter, this core uses ProcessSample() to filter one sample at a time as called
	//       by its module/owner
	void TPTLPLadderFilterCore::Render(unsigned int numSamples)
	{

	}

	void TPTLPLadderFilterCore::DoNoteOn(NoteEvent event)
	{
		m_S = 0.0f;

		float g = tan(2 * SynthBase::Pi * m_CutoffFreq / (float)m_SampleRate * m_Gain / 2);
		m_G = g / (1 + g);
	}

	void TPTLPLadderFilterCore::DoNoteOff(NoteEvent event)
	{

	}

	float TPTLPLadderFilterCore::ProcessSample(float input)
	{
		float output = 0.0f;
		float x = 0.0f;
		float y = 0.0f;
		float v = 0.0f;

		x = input;
		v = (x - m_S) * m_G;
		y = v + m_S;
		m_S = y + v;
	
		output = y;
		return output;
	}

}