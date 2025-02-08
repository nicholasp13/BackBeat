#pragma once

#include "BackBeat/Audio/Audio.h"
#include "BackBeat/Core/Log.h"
#include "BackBeat/Core/Timer.h"
#include "SynthParameters.h"
namespace BackBeat {

	namespace SynthBase {
		// ---- CONSTANTS ---- //

		// MISC
		constexpr unsigned int NumOscillators = 4;
		constexpr float Pi                    = 3.141592653589793f; // Float(32 bit) precise pi;
		constexpr float CircleDegrees         = 360.0f;
		constexpr int NotesInOctave           = 12;
		constexpr float LowerOctave           = 0.5f;
		constexpr float IncreaseOctave        = 2.0f;
		constexpr float LowestOctave          = 0.03125f;
		constexpr float HighestOctave         = 32.0f;
		constexpr int LowestOctaveSynth       = -1;
		constexpr int HighestOctaveSynth      = 9;
		constexpr float DeltaCentsHertz       = 0.004860f; // The smallest value of a cent in hz, Formula (C_Minus1 - C_SHARP_MINUS_1_NOTE) / 100.0f

		// MUSIC NOTES in hertz
		// Lowest note
		constexpr float CMinus1Frequency = 8.176f;
		// Middle/4th octave notes and in hertz)
		constexpr float C4Frequency      = 261.6f;
		constexpr float CSharp4Frequency = 277.2f;
		constexpr float D4Frequency      = 293.7f;
		constexpr float DSharp4Frequency = 311.1f;
		constexpr float E4Frequency      = 329.6f;
		constexpr float F4Frequency      = 349.2f;
		constexpr float FSharp4Frequency = 370.0f;
		constexpr float G4Frequency      = 392.0f;
		constexpr float GSharp4Frequency = 415.0f;
		constexpr float A4Frequency      = 440.0f;
		constexpr float ASharp4Frequency = 466.2f;
		constexpr float B4Frequency      = 493.9f;
		// Highest note
		constexpr float G9Frequency      = 12543.9f;

		// -------- CONSTANTS FOR PARAMETERS --------- //
		// MIDI Manufacturer's Association (MMA) min, max, and default values for Downloadable Sounds (DLS)
		// Website: https://midi.org/dls
		// Specification level 1
		// LFOs
		constexpr float LFOMinDelay                 = 0.1f; // in seconds
		constexpr float LFOMaxDelay                 = 10.0f;
		constexpr float LFOFrequencyDefault         = 5.0f;
		constexpr float LFOFrequencyMin             = 0.1f;
		constexpr float LFOFrequencyMax             = 10.0f;
		constexpr float LFOAttentuationDefault      = 0.0f;
		constexpr float LFOAttentuationMin          = 0.0f;
		constexpr float LFOAttentuationMax          = 12.0f;
		constexpr float LFOPitchDefault             = 0.0f;
		constexpr float LFOPitchMin                 = -1200.0f;
		constexpr float LFOPitchMax                 = 1200.0f;
		constexpr float LFOModToAttentuationDefault = 0.0f;
		constexpr float LFOModToAttentuation        = 0.0f;
		constexpr float LFOModToAttentuationMax     = 12.0f;
		constexpr float LFOModToPitchDefault        = 0.0f;
		constexpr float LFOModToPitchMin            = -1200.0f;
		constexpr float LFOModToPitchMax            = 1200.0f;
		// EG1
		constexpr float EG1AttackTimeDefault       = 0.0f;
		constexpr float EG1AttackTimeMin           = 0.0f;
		constexpr float EG1AttackTimeMax           = 20.0f;
		constexpr float EG1DecayTimeDefault        = 0.0f;
		constexpr float EG1DecayTimeMin            = 0.0f;
		constexpr float EG1DecayTimeMax            = 40.0f;
		constexpr float EG1SustainLevelDefault     = 1.0f;
		constexpr float EG1SustainLevelMin         = 0.0f;
		constexpr float EG1SustainLevelMax         = 1.0f;
		constexpr float EG1ReleaseTimeDefault      = 0.0f;
		constexpr float EG1ReleaseTimeMin          = 0.0f;
		constexpr float EG1ReleaseTimeMax          = 20.0f;
		constexpr float EG1VelocityToAttackDefault = 0.0f;
		constexpr float EG1VelocityToAttackMin     = 0.0f;
		constexpr float EG1VelocityToAttackMax     = 20.0f;
		constexpr float EG1KeyToDecayDefault       = 0.0f;
		constexpr float EG1KeyToDecayMin           = 0.0f;
		constexpr float EG1KeyToDecayMax           = 20.0f;
		// EG2
		constexpr float EG2AttackTimeDefault       = 0.0f;
		constexpr float EG2AttackTimeMin           = 0.0f;
		constexpr float EG2AttackTimeMax           = 20.0f;
		constexpr float EG2DecayTimeDefault        = 0.0f;
		constexpr float EG2DecayTimeMin            = 0.0f;
		constexpr float EG2DecayTimeMax            = 40.0f;
		constexpr float EG2SustainLevelDefault     = 1.0f;
		constexpr float EG2SustainLevelMin         = 0.0f;
		constexpr float EG2SustainLevelMax         = 1.0f;
		constexpr float EG2ReleaseTimeDefault      = 0.0f;
		constexpr float EG2ReleaseTimeMin          = 0.0f;
		constexpr float EG2ReleaseTimeMax          = 20.0f;
		constexpr float EG2VelocityToAttackDefault = 0.0f;
		constexpr float EG2VelocityToAttackMin     = 0.0f;
		constexpr float EG2VelocityToAttackMax     = 20.0f;
		constexpr float EG2KeyToDecayDefault       = 0.0f;
		constexpr float EG2KeyToDecayMin           = 0.0f;
		constexpr float EG2KeyToDecayMax           = 20.0f;
		constexpr float EG2ToPitchDefault          = 0.0f;
		constexpr float EG2ToPitchMin              = -1200.0f;
		constexpr float EG2ToPitchMax              = 1200.0f;
		// DCA
		constexpr float AmpDefault    = 0.50; // NOTE: This should be 0.70 when pan is implemented properly with a nonlinear scale
		constexpr float PanDefault    = 0.0f;
		constexpr float PanMin        = -0.50f;
		constexpr float PanMax        = 0.50f;

		// MIDI Manufacturer's Association (MMA) min, max, and default values for Downloadable Sounds (DLS)
		// Specification level 2
		// Website: https://midi.org/dls
		// Filters
		constexpr float FilterCutoffMin = 200.0f;      // Sample rate / 240
		constexpr float FilterCutoffMax = 8000.0f;     // Sample rate / 6

		// BackBeat default values
		// WaveOsc
		constexpr float WaveAmpDefault      = 0.0f;
		constexpr float WaveOctaveDefault   = 1.0f;
		constexpr float WaveDutyCyleDefault = 0.5f;
		constexpr float WaveDutyCyleMin     = 0.0f; // Only half a cycle (50% or 0.50f) matters as it sounds
		constexpr float WaveDutyCyleMax     = 1.0f; // the same i.e. .05 and 0.95 are the same audibly
		constexpr float WaveDetuneDefault   = 0.0f; // In centz
		constexpr float WaveDetuneMin       = -50.0f;
		constexpr float WaveDetuneMax       = 50.0f;
		// LPLadderFilter
		// based off Will Pirkle's design for a ladder filter
		// link: https://willpirkle.com/Downloads/Addendum_A1_Moog.pdf
		constexpr float LadderFilterQDefault = 1.0f;
		constexpr float LadderFilterQMin = 1.0f;
		constexpr float LadderFilterQMax = 10.0f;
		constexpr float LadderFilterBassBoostDefault = 0.0f;
		constexpr float LadderFilterBassBoostMin = 0.0f;
		constexpr float LadderFilterBassBoostMax = 100.0f;

		// ------------------------------------------- //

		// ---- HELPER FUNCTIONS ---- //

		// Time should be in milliseconds
		static unsigned int MsToSamples(float time, unsigned int sampleRate)
		{
			unsigned int SamplesPerMS = sampleRate / 1000;
			unsigned int timePos = (unsigned int)time % 1000;
			return timePos * SamplesPerMS;
		}

		static int GetMidiChannel(MIDIEvent event, bool noteOn)
		{
			if (noteOn)
			{
				return (int)event.status - (int)MIDI::ChannelOn1 + 1;
			}
			else 
			{
				return (int)event.status - (int)MIDI::ChannelOff1 + 1;
			}
		}

		static byte GetChannelOff(byte channelOn)
		{
			return channelOn - byte(0x10);
		}

		// Note num to pitch: (A4_Freq) * 2 ^ ((noteNum - A4_Num) / 12)
		static float MIDIToFreq(byte midiNote)
		{
			return A4Frequency * pow(2.0f, ((float)midiNote - (float)MIDI::A4) / 12.0f);
		}

		static NoteEvent ConvertEvent(MIDIEvent event)
		{
			NoteEvent nEvent = {
				.noteOn = false,
				.channel = -1,
				.note = 0.0f,
				.midiNote = event.data1,
				.velocity = event.data2
			};

			nEvent.noteOn = Audio::IsNoteOn(event);
			nEvent.channel = GetMidiChannel(event, nEvent.noteOn);

			if (event.data1 > MIDI::G9)
				nEvent.note = G9Frequency;
			else
				nEvent.note = MIDIToFreq(event.data1);

			return nEvent;
		}
	}
}