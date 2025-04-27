#pragma once

// Same filter as the one used in Synthesizer.
// Process per sample not in audio frames

#include "BackBeat/Audio/Audio.h"
#include "BackBeat/Audio/Instruments/Synth/SynthBase.h"
namespace BackBeat {

	struct HighPassFilterFxParameters
	{
		bool isOn = false;
		float cutoff = SynthBase::FilterCutoffMin;

		HighPassFilterFxParameters& operator = (const HighPassFilterFxParameters& rhs)
		{
			isOn = rhs.isOn;
			cutoff = rhs.cutoff;
			return *this;
		}

		bool operator == (const HighPassFilterFxParameters& rhs)
		{
			const float delta = 0.001f;
			return Audio::EqualsFloat(cutoff, rhs.cutoff, delta) && (isOn == rhs.isOn);
		}

		bool operator != (const HighPassFilterFxParameters& rhs)
		{
			return !(*this == rhs);
		}

	};

	class HighPassFilterFx
	{
	public:
		HighPassFilterFx();
		~HighPassFilterFx();

		void Update(HighPassFilterFxParameters params);
		void Reset();
		float ProcessSample(float sample);

	private:
		unsigned int m_SampleRate = Audio::DefaultSampleRate;
		float m_Gain = 1.0f; // Represented by T in the transform method, defaulted to 1 and not subject to change yet
		float m_CutoffFreq = SynthBase::FilterCutoffMin;

		// Helper Variables used in transform method
		float m_g = 0.0f;
		float m_GHp = 0.0f;	// GHp = 1 / ( 1 + g)
		float m_S = 0.0f;   // Contains the state of z^-1 which is the last output of the transform 

		HighPassFilterFxParameters m_Params = HighPassFilterFxParameters();

	};


}
