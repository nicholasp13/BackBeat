#pragma once

// Same filter as the one used in Synthesizer.
// Process per sample not in audio frames

#include "BackBeat/Audio/Audio.h"
#include "BackBeat/Audio/Instruments/Synth/SynthBase.h"
namespace BackBeat {

	struct LowPassFilterFxParameters
	{
		bool isOn = false;
		float cutoff = SynthBase::FilterCutoffMax;

		LowPassFilterFxParameters& operator = (const LowPassFilterFxParameters& rhs)
		{
			isOn = rhs.isOn;
			cutoff = rhs.cutoff;
			return *this;
		}

		bool operator == (const LowPassFilterFxParameters& rhs)
		{
			const float delta = 0.001f;
			return Audio::EqualsFloat(cutoff, rhs.cutoff, delta) && (isOn == rhs.isOn);
		}

		bool operator != (const LowPassFilterFxParameters& rhs)
		{
			return !(*this == rhs);
		}

	};

	class LowPassFilterFx
	{
	public:
		LowPassFilterFx();
		~LowPassFilterFx();

		void Update(LowPassFilterFxParameters params);
		void Reset();
		float ProcessSample(float sample);

	private:
		unsigned int m_SampleRate = Audio::DefaultSampleRate;
		float m_Gain = 1.0f; // Represented by T in the transform method, defaulted to 1 and not subject to change yet
		float m_CutoffFreq = SynthBase::FilterCutoffMax;

		// Helper Variables used in transform method
		float m_G = 0.0f; // G = g / ( 1 + g), g = tan( 2 * pi * cutoff freq * constant T / 2) where T is usually 1 / sample rate and some gain
		float m_S = 0.0f; // Contains the state of z^-1 which is the last output of the transform 

		LowPassFilterFxParameters m_Params = LowPassFilterFxParameters();

	};


}