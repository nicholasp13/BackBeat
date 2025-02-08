#include "bbpch.h"

#include "LowPassLadderFilter.h"
namespace BackBeat {

	LowPassLadderFilter::LowPassLadderFilter(unsigned int sampleRate, unsigned int bufferSize, 
		std::shared_ptr<float[]> buffer, std::shared_ptr<LadderFilterParameters> params)
		: 
		m_BufferSize(bufferSize),
		m_K(0.0f),
		m_Params(params),
		m_Buffer(buffer),
		m_LowPassFilter1(std::make_shared<TPTLPLadderFilterCore>(sampleRate)),
		m_LowPassFilter2(std::make_shared<TPTLPLadderFilterCore>(sampleRate)),
		m_LowPassFilter3(std::make_shared<TPTLPLadderFilterCore>(sampleRate)),
		m_LowPassFilter4(std::make_shared<TPTLPLadderFilterCore>(sampleRate)),
		m_ModInput(std::make_unique<Modulator>(bufferSize)),
		m_Type(ModuleType::LowPassFilter)
	{

	}

	LowPassLadderFilter::~LowPassLadderFilter()
	{

	}

	void LowPassLadderFilter::Reset(unsigned int sampleRate)
	{
		m_LowPassFilter1->Reset(sampleRate);
		m_LowPassFilter2->Reset(sampleRate);
		m_LowPassFilter3->Reset(sampleRate);
		m_LowPassFilter4->Reset(sampleRate);
	}

	void LowPassLadderFilter::Update()
	{
		CalculateCoefficients();
		m_LowPassFilter1->Update();
		m_LowPassFilter2->Update();
		m_LowPassFilter3->Update();
		m_LowPassFilter4->Update();
	}

	void LowPassLadderFilter::Render(unsigned int numSamples)
	{
		if (!m_Params->isOn)
			return;

		Update();

		float output = 0.0f;
		for (unsigned int i = 0; i < numSamples * Audio::Stereo; i += Audio::Stereo)
		{
			output = ProcessSample(m_Buffer[i]);
			m_Buffer.get()[i] = output;
			m_Buffer.get()[i + 1] = output;
		}

		m_ModInput->FlushBuffer(numSamples, 1.0f);
	}

	void LowPassLadderFilter::DoNoteOn(NoteEvent event)
	{
		m_ModInput->FlushBuffer(m_BufferSize / Audio::Stereo, 1.0f);

		m_LowPassFilter1->DoNoteOn(event);
		m_LowPassFilter2->DoNoteOn(event);
		m_LowPassFilter3->DoNoteOn(event);
		m_LowPassFilter4->DoNoteOn(event);
	}

	void LowPassLadderFilter::DoNoteOff(NoteEvent event)
	{
		m_LowPassFilter1->DoNoteOff(event);
		m_LowPassFilter2->DoNoteOff(event);
		m_LowPassFilter3->DoNoteOff(event);
		m_LowPassFilter4->DoNoteOff(event);
	}

	void LowPassLadderFilter::CalculateCoefficients()
	{
		// Maps Q from 1 -> 10 to 0 -> 4
		m_K = (4.0f) * (m_Params->Q - 1.0f) / 9.0f;

		// Makes sure cutoff frequencies are all set
		float cutoff = m_Params->cutoff * m_ModInput->GetBuffer()[0];
		Audio::Bound(cutoff, SynthBase::FilterCutoffMin, SynthBase::FilterCutoffMax);

		m_LowPassFilter1->SetCuttoffFreq(cutoff);
		m_LowPassFilter2->SetCuttoffFreq(cutoff);
		m_LowPassFilter3->SetCuttoffFreq(cutoff);
		m_LowPassFilter4->SetCuttoffFreq(cutoff);
	}

	float LowPassLadderFilter::ProcessSample(float input)
	{
		float output = 0.0f;

		// Get S values
		float s1 = m_LowPassFilter1->GetS_Value();
		float s2 = m_LowPassFilter2->GetS_Value();
		float s3 = m_LowPassFilter3->GetS_Value();
		float s4 = m_LowPassFilter4->GetS_Value();

		// Setup coefficients
		float G = m_LowPassFilter1->GetG_Value();
		float dSigma = G * G * G * s1 + G * G * s2 + G * s3 + s4;
		float alpha0 = 1.0f / (1.0f + m_K * G * G * G * G);

		// Bass compensation
		float compensationGain = 1.0f + (m_Params->bassBoostPercent / 100.0f) * m_K;

		// Scale input before calculating
		float x = input * compensationGain;

		// Calculate first input value
		float u = (x - m_K * dSigma) * alpha0;

		float lpf1 = m_LowPassFilter1->ProcessSample(u);
		float lpf2 = m_LowPassFilter1->ProcessSample(lpf1);
		float lpf3 = m_LowPassFilter1->ProcessSample(lpf2);
		float lpf4 = m_LowPassFilter1->ProcessSample(lpf3);

		// Add peak limiter here if needed

		output = lpf4;

		return output;
	}
}