#pragma once

/**
* SynthVoice is a single object responsible for producing one sound similar to a singular key on a piano
* but capabable of any note with a number of modulations. All SynthVoices have the same SynthModules with the same 
* ModuleCores but are seperate objects stored in a vector. Only the VoiceParameters are shared between the seperate Voices
* so that they all do the same modulations given by the GUI input to those parameters. SynthVoices architecture can be
* customized heavily through changing their SynthModules and ModuleCores. 
*
* This SynthVoice architecture:
*  - 4 oscillators (Oscs)
*    - 1 main oscillator (m_Osc1) with an LFO to modulate its sound
*    - 3 other oscillators to produce a chorus like effect
*    - 5 different waveforms: Sin, Triangle, Square, SawtoothUp, SawtoothDown
*    - Produce the same note but can be scaled by octaves (pitch shifting on the backburner)
*    - Seperate amp controllers to change the balance between the 4
*  - Digitally Controlled Amplifier (DCA)
*    - Controls the amplitude/volume of the SynthVoice
*    - Controls panning (left and right channel mixing for STEREO)
*    - Controlled by parameters and Amp EG
*  - 2 Linear Envelope Generators (EGs)
*    - Amp Eg connected directly to the DCA to produce more natural sound shaping
*    - Another Linear EG to be connected to a filter or another modulator (not implemented / on the backburner)
*  - 1 Low Frequency Oscillator (LFO)
*    - Currently one singular LFO connected to the main oscillator (m_Osc1) (more to be added / on the backburner)
*  - Low pass filter (LPFilter)
*    - Basic first degree filter mimicking a resistor capacitor (RC) filter using a bilinear transform
*  - High pass filter (HPFilter)
*    - Basic first degree filter mimicking a capacitor resistor (CR) filter using a bilinear transform
* 
* Most SynthVoice function calls i.e. Reset(), Render(), DoNoteOff(), and DoNoteOn() call the corresponding 
* function in their SynthModules which further make the corresponding functions calls to their cores which do the 
* main work. The final output of the SynthModules is outputted to a single float buffer shared between most 
* SynthModules (Oscs, Fitlers, and DCA specifically) which the SynthVoice then outputs additively to the main 
* float buffer shared between all SynthVoices and the SynthEngine. 
*
* NOTE: - Buffer size of most buffers is just the sample rate 48,000 which is not multiplied by the number of channels 
*       so the buffer holds only half a second of data. This is sufficient as only LFOs require a buffer size larger 
*       than half a second as only entire waves are stored in buffers. Most other ModuleCores are implemented by
*       increments or multiplicatives. The only other possible cause for a larger buffer size would be a change in
*       sample rate or the Renderer calls for more data but that should be handled by the Renderer to make sure
*       it does not call for more than a half second of data as that create too much latency for a Synth
*       - Calls in Render() function must be done in a specific order of ModularSources, ModularMatrix (not implemented),
*       then Modular Destinations. Furthermore, ModularSources can be called in any order (subject to change after
*       ModularMatrix is implemented). But ModularDestinations must be called in order of Oscs, Filters, DCA. DCA
*       must ALWAYS be rendered last. Specification of what are ModularSources and ModularDestinations are in the
*       SynthParameters.h file (location subject to change) Function call order does not matter for all other functions.
*/

// TODO:
//	Add EG for filters specifically
//  Implement ModularMatrix

// #include "BackBeat/Audio/SynthIO/RenderInfo.h"
#include "BackBeat/Audio/Instruments/Module.h"
#include "BackBeat/Audio/Instruments/Synth/SynthEngine/SynthModules/DCA.h"
#include "BackBeat/Audio/Instruments/Synth/SynthEngine/SynthModules/WaveOscillator.h"
#include "BackBeat/Audio/Instruments/Synth/SynthEngine/SynthModules/LinearEG.h"
#include "BackBeat/Audio/Instruments/Synth/SynthEngine/SynthModules/AmpEG.h"
#include "BackBeat/Audio/Instruments/Synth/SynthEngine/SynthModules/LowFrequencyOscillator.h"
#include "BackBeat/Audio/Instruments/Synth/SynthEngine/SynthModules/TPTLowPassFilter.h"
#include "BackBeat/Audio/Instruments/Synth/SynthEngine/SynthModules/TPTHighPassFilter.h"
#include "BackBeat/Audio/Instruments/Synth/SynthBase.h"
namespace BackBeat {

	class SynthVoice
	{
	public:
		SynthVoice(unsigned int sampleRate, std::shared_ptr<float[]> outputBuffer, std::shared_ptr<VoiceParameters> params);
		~SynthVoice();

		// Virtual for implementation of subclasses if needed
		virtual void Reset(unsigned int sampleRate);
		virtual void Update();
		virtual void Render(unsigned int numSamples);
		virtual void ProcessMIDIEvent(MIDIEvent event);

		inline virtual int GetChannel() { return m_Channel; }
		inline virtual byte GetNote() { return m_NotePressed; }
		inline virtual bool IsActive() { return m_Active; }

	private:
		unsigned int m_SampleRate;
		bool m_Active;
		int m_Channel;
		byte m_NotePressed;

		std::shared_ptr<VoiceParameters> m_Params;
		std::shared_ptr<float[]> m_InputBuffer;
		std::shared_ptr<float[]> m_OutputBuffer;
		std::unique_ptr<DCA> m_DCA;
		// NOTE: Might change to one SynthModule with Four Oscillator cores
		//       Might change to stack allocation for objects for performance
		std::unique_ptr<WaveOscillator> m_Osc1;
		std::unique_ptr<WaveOscillator> m_Osc2;
		std::unique_ptr<WaveOscillator> m_Osc3;
		std::unique_ptr<WaveOscillator> m_Osc4;
		// std::unique_ptr<LinearEG> m_EG; // TODO: Link this EG to one of or both of the filters
		std::unique_ptr<AmpEG> m_AmpEG;
		std::unique_ptr<LowFrequencyOscillator> m_LFO1;
		std::unique_ptr<TPTLowPassFilter> m_LPFilter;
		std::unique_ptr<TPTHighPassFilter> m_HPFilter;

	private:
		void DoNoteOn(NoteEvent event);
		void DoNoteOff(NoteEvent event);
		void InitModules();
	};
}