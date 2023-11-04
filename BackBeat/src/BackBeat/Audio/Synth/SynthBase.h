#pragma once

#include "BackBeat/Audio/Audio.h"
#include "BackBeat/Core/Log.h"
#include "BackBeat/Core/Timer.h"
#include "SynthParameters.h"
namespace BackBeat {

// CONSTANTS
#define FLOAT_SIZE 4
#define PI 3.141592653589793f // Float(32 bit) precise pi
#define CIRCLE_DEGREES 360.0f
#define NOTES_IN_OCTAVE 12
#define LOWER_OCTAVE 0.5f
#define HIGHER_OCTAVE 2.0f
#define LOWEST_OCTAVE 0.03125f
#define HIGHEST_OCTAVE 32.0f

// MUSIC NOTES(middle/4th octave notes and in hertz)
#define C_4_NOTE       261.6f
#define C_SHARP_4_NOTE 277.2f
#define D_4_NOTE       293.7f
#define D_SHARP_4_NOTE 311.1f
#define E_4_NOTE       329.6f
#define F_4_NOTE       349.2f
#define F_SHARP_4_NOTE 370.0f
#define G_4_NOTE       392.0f
#define G_SHARP_4_NOTE 415.0f
#define A_4_NOTE       440.0f
#define A_SHARP_4_NOTE 466.2f
#define B_4_NOTE       493.9f

typedef unsigned char byte;

// -------- CONSTANTS FOR MIDI EVENTS --------- //
// Website: https://midi.org/specifications/midi-reference-tables/
// TODO: Add other constants as needed

// 1ST (STATUS) BYTE CONSTANTS
#define CHANNEL_1_NOTE_OFF (byte)0x80
#define CHANNEL_2_NOTE_OFF (byte)0x81
#define CHANNEL_3_NOTE_OFF (byte)0x82
#define CHANNEL_4_NOTE_OFF (byte)0x83
#define CHANNEL_5_NOTE_OFF (byte)0x84
#define CHANNEL_6_NOTE_OFF (byte)0x85
#define CHANNEL_7_NOTE_OFF (byte)0x86
#define CHANNEL_8_NOTE_OFF (byte)0x87
#define CHANNEL_9_NOTE_OFF (byte)0x88
#define CHANNEL_10_NOTE_OFF (byte)0x89
#define CHANNEL_11_NOTE_OFF (byte)0x8A
#define CHANNEL_12_NOTE_OFF (byte)0x8B
#define CHANNEL_13_NOTE_OFF (byte)0x8C
#define CHANNEL_14_NOTE_OFF (byte)0x8D
#define CHANNEL_15_NOTE_OFF (byte)0x8E
#define CHANNEL_16_NOTE_OFF (byte)0x8F

#define CHANNEL_1_NOTE_ON (byte)0x90
#define CHANNEL_2_NOTE_ON (byte)0x91
#define CHANNEL_3_NOTE_ON (byte)0x92
#define CHANNEL_4_NOTE_ON (byte)0x93
#define CHANNEL_5_NOTE_ON (byte)0x94
#define CHANNEL_6_NOTE_ON (byte)0x95
#define CHANNEL_7_NOTE_ON (byte)0x96
#define CHANNEL_8_NOTE_ON (byte)0x97
#define CHANNEL_9_NOTE_ON (byte)0x98
#define CHANNEL_10_NOTE_ON (byte)0x99
#define CHANNEL_11_NOTE_ON (byte)0x9A
#define CHANNEL_12_NOTE_ON (byte)0x9B
#define CHANNEL_13_NOTE_ON (byte)0x9C
#define CHANNEL_14_NOTE_ON (byte)0x9D
#define CHANNEL_15_NOTE_ON (byte)0x9E
#define CHANNEL_16_NOTE_ON (byte)0x9F

#define MIDI_START 0xFA
#define MIDI_CONTINUE 0xFB
#define MIDI_STOP 0xFC

// 2ND BYTE CONSTANTS
//	MIDI NOTES
//	Note: Range is based from Korg and Roland synths' range
//	-1st octave (0-11)
#define MIDI_NOTE_C_MINUS_1         (byte)0x00 // Lowest note
#define MIDI_NOTE_C_SHARP_MINUS_1   (byte)0x01
#define MIDI_NOTE_D_MINUS_1         (byte)0x02
#define MIDI_NOTE_D_SHARP_MINUS_1   (byte)0x03
#define MIDI_NOTE_E_MINUS_1         (byte)0x04
#define MIDI_NOTE_F_MINUS_1         (byte)0x05
#define MIDI_NOTE_F_SHARP_MINUS_1   (byte)0x06
#define MIDI_NOTE_G_MINUS_1         (byte)0x07
#define MIDI_NOTE_G_SHARP_MINUS_1   (byte)0x08
#define MIDI_NOTE_A_MINUS_1         (byte)0x09
#define MIDI_NOTE_A_SHARP_MINUS_1   (byte)0x0A
#define MIDI_NOTE_B_MINUS_1         (byte)0x0B
//	0th octave (12-23)
#define MIDI_NOTE_C_0       (byte)0x0C
#define MIDI_NOTE_C_SHARP_0 (byte)0x0D
#define MIDI_NOTE_D_0       (byte)0xoE
#define MIDI_NOTE_D_SHARP_0 (byte)0x0F
#define MIDI_NOTE_E_0       (byte)0x10
#define MIDI_NOTE_F_0       (byte)0x11
#define MIDI_NOTE_F_SHARP_0 (byte)0x12
#define MIDI_NOTE_G_0       (byte)0x13
#define MIDI_NOTE_G_SHARP_0 (byte)0x14
#define MIDI_NOTE_A_0       (byte)0x15
#define MIDI_NOTE_A_SHARP_0 (byte)0x16
#define MIDI_NOTE_B_0       (byte)0x17
	//	1st octave (24-35)
#define MIDI_NOTE_C_1       (byte)0x18
#define MIDI_NOTE_C_SHARP_1 (byte)0x19
#define MIDI_NOTE_D_1       (byte)0x1A
#define MIDI_NOTE_D_SHARP_1 (byte)0x1B
#define MIDI_NOTE_E_1       (byte)0x1C
#define MIDI_NOTE_F_1       (byte)0x1D
#define MIDI_NOTE_F_SHARP_1 (byte)0x1E
#define MIDI_NOTE_G_1       (byte)0x1F
#define MIDI_NOTE_G_SHARP_1 (byte)0x20
#define MIDI_NOTE_A_1       (byte)0x21
#define MIDI_NOTE_A_SHARP_1 (byte)0x22
#define MIDI_NOTE_B_1       (byte)0x23
	//	2nd octave (36-47)
#define MIDI_NOTE_C_2       (byte)0x24
#define MIDI_NOTE_C_SHARP_2 (byte)0x25
#define MIDI_NOTE_D_2       (byte)0x26
#define MIDI_NOTE_D_SHARP_2 (byte)0x27
#define MIDI_NOTE_E_2       (byte)0x28
#define MIDI_NOTE_F_2       (byte)0x29
#define MIDI_NOTE_F_SHARP_2 (byte)0x2A
#define MIDI_NOTE_G_2       (byte)0x2B
#define MIDI_NOTE_G_SHARP_2 (byte)0x2C
#define MIDI_NOTE_A_2       (byte)0x2D
#define MIDI_NOTE_A_SHARP_2 (byte)0x2E
#define MIDI_NOTE_B_2       (byte)0x2F
//	3rd octave (48-59)
#define MIDI_NOTE_C_3       (byte)0x30
#define MIDI_NOTE_C_SHARP_3 (byte)0x31
#define MIDI_NOTE_D_3       (byte)0x32
#define MIDI_NOTE_D_SHARP_3 (byte)0x33
#define MIDI_NOTE_E_3       (byte)0x34
#define MIDI_NOTE_F_3       (byte)0x35
#define MIDI_NOTE_F_SHARP_3 (byte)0x36
#define MIDI_NOTE_G_3       (byte)0x37
#define MIDI_NOTE_G_SHARP_3 (byte)0x38
#define MIDI_NOTE_A_3       (byte)0x39
#define MIDI_NOTE_A_SHARP_3 (byte)0x3A
#define MIDI_NOTE_B_3       (byte)0x3B
//	4th octave (60-71)
#define MIDI_NOTE_C_4       (byte)0x3C
#define MIDI_NOTE_C_SHARP_4 (byte)0x3D
#define MIDI_NOTE_D_4       (byte)0x3E
#define MIDI_NOTE_D_SHARP_4 (byte)0x3F
#define MIDI_NOTE_E_4       (byte)0x40
#define MIDI_NOTE_F_4       (byte)0x41
#define MIDI_NOTE_F_SHARP_4 (byte)0x42
#define MIDI_NOTE_G_4       (byte)0x43
#define MIDI_NOTE_G_SHARP_4 (byte)0x44
#define MIDI_NOTE_A_4       (byte)0x45
#define MIDI_NOTE_A_SHARP_4 (byte)0x46
#define MIDI_NOTE_B_4       (byte)0x47
//	5th octave (72-83)
#define MIDI_NOTE_C_5       (byte)0x48
#define MIDI_NOTE_C_SHARP_5 (byte)0x49
#define MIDI_NOTE_D_5       (byte)0x4A
#define MIDI_NOTE_D_SHARP_5 (byte)0x4B
#define MIDI_NOTE_E_5       (byte)0x4C
#define MIDI_NOTE_F_5       (byte)0x4D
#define MIDI_NOTE_F_SHARP_5 (byte)0x4E
#define MIDI_NOTE_G_5       (byte)0x4F
#define MIDI_NOTE_G_SHARP_5 (byte)0x50
#define MIDI_NOTE_A_5       (byte)0x51
#define MIDI_NOTE_A_SHARP_5 (byte)0x52
#define MIDI_NOTE_B_5       (byte)0x53
//	6th octave (84-95)
#define MIDI_NOTE_C_6       (byte)0x54
#define MIDI_NOTE_C_SHARP_6 (byte)0x55
#define MIDI_NOTE_D_6       (byte)0x56
#define MIDI_NOTE_D_SHARP_6 (byte)0x57
#define MIDI_NOTE_E_6       (byte)0x58
#define MIDI_NOTE_F_6       (byte)0x59
#define MIDI_NOTE_F_SHARP_6 (byte)0x5A
#define MIDI_NOTE_G_6       (byte)0x5B
#define MIDI_NOTE_G_SHARP_6 (byte)0x5C
#define MIDI_NOTE_A_6       (byte)0x5D
#define MIDI_NOTE_A_SHARP_6 (byte)0x5E
#define MIDI_NOTE_B_6       (byte)0x5F
//	7th octave (96-107)
#define MIDI_NOTE_C_7       (byte)0x60
#define MIDI_NOTE_C_SHARP_7 (byte)0x61
#define MIDI_NOTE_D_7       (byte)0x62
#define MIDI_NOTE_D_SHARP_7 (byte)0x63
#define MIDI_NOTE_E_7       (byte)0x64
#define MIDI_NOTE_F_7       (byte)0x65
#define MIDI_NOTE_F_SHARP_7 (byte)0x66
#define MIDI_NOTE_G_7       (byte)0x67
#define MIDI_NOTE_G_SHARP_7 (byte)0x68
#define MIDI_NOTE_A_7       (byte)0x69
#define MIDI_NOTE_A_SHARP_7 (byte)0x6A
#define MIDI_NOTE_B_7       (byte)0x6B
//	8th octave (108-119)
#define MIDI_NOTE_C_8       (byte)0x6C
#define MIDI_NOTE_C_SHARP_8 (byte)0x6D
#define MIDI_NOTE_D_8       (byte)0x6E
#define MIDI_NOTE_D_SHARP_8 (byte)0x7F
#define MIDI_NOTE_E_8       (byte)0x70
#define MIDI_NOTE_F_8       (byte)0x71
#define MIDI_NOTE_F_SHARP_8 (byte)0x72
#define MIDI_NOTE_G_8       (byte)0x73
#define MIDI_NOTE_G_SHARP_8 (byte)0x74
#define MIDI_NOTE_A_8       (byte)0x75
#define MIDI_NOTE_A_SHARP_8 (byte)0x76
#define MIDI_NOTE_B_8       (byte)0x77
//	9th octave (120-127)
#define MIDI_NOTE_C_9       (byte)0x78
#define MIDI_NOTE_C_SHARP_9 (byte)0x79
#define MIDI_NOTE_D_9       (byte)0x7A
#define MIDI_NOTE_D_SHARP_9 (byte)0x7B
#define MIDI_NOTE_E_9       (byte)0x7C
#define MIDI_NOTE_F_9       (byte)0x7D
#define MIDI_NOTE_F_SHARP_9 (byte)0x7E
#define MIDI_NOTE_G_9       (byte)0x7F       // Highest note

//	OTHER (added as needed)

// 3RD BYTE CONSTANTS
//	NOTE VELOCITY (0-127)
#define MIN_VELOCITY 0x00 // SAME AS NOTE OFF
#define MAX_VELOCITY 0x7F

//	OTHER (added as needed)

// MIDI Manufacturer's Association (MMA) min, max, and default values for Downloadable Sounds (DLS)
// Specification level 1
// LFOs
#define LFO_FREQ_DEFAULT			5.0f
#define LFO_FREQ_MIN				0.1f
#define LFO_FREQ_MAX				10.0f
#define LFO_ATT_DEFAULT				0.0f
#define LFO_ATT_MIN					0.0f
#define LFO_ATT_MAX					12.0f
#define LFO_PITCH_DEFAULT			0.0f
#define LFO_PITCH_MIN				-1200.0f
#define LFO_PITCH_MAX				1200.0f
#define LFO_MOD_TO_ATT_DEFAULT		0.0f
#define LFO_MOD_TO_ATT_MIN			0.0f
#define LFO_MOD_TO_ATT_MAX			12.0f
#define LFO_MOD_TO_PITCH_DEFAULT	0.0f
#define LFO_MOD_TO_PITCH_MIN		-1200.0f
#define LFO_MOD_TO_PITCH_MAX		1200.0f
// EG1
#define EG1_ATTACK_TIME_DEFAULT		0.0f
#define EG1_ATTACK_TIME_MIN			0.0f
#define EG1_ATTACK_TIME_MAX			20.0f
#define EG1_DECAY_TIME_DEFAULT		0.0f
#define EG1_DECAY_TIME_MIN			0.0f
#define EG1_DECAY_TIME_MAX			40.0f
#define EG1_SUSTAIN_LEVEL_DEFAULT	1.0f
#define EG1_SUSTAIN_LEVEL_MIN		0.0f
#define EG1_SUSTAIN_LEVEL_MAX		1.0f
#define EG1_RELEASE_TIME_DEFAULT	0.0f
#define EG1_RELEASE_TIME_MIN		0.0f
#define EG1_RELEASE_TIME_MAX		20.0f
#define EG1_VEL_TO_ATTACK_DEFAULT	0.0f
#define EG1_VEL_TO_ATTACK_MIN		0.0f
#define EG1_VEL_TO_ATTACK_MAX		20.0f
#define EG1_KEY_TO_DECAY_DEFAULT	0.0f
#define EG1_KEY_TO_DECAY_MIN		0.0f
#define EG1_KEY_TO_DECAY_MAX		20.0f
// EG2
#define EG2_ATTACK_TIME_DEFAULT		0.0f
#define EG2_ATTACK_TIME_MIN			0.0f
#define EG2_ATTACK_TIME_MAX			20.0f
#define EG2_DECAY_TIME_DEFAULT		0.0f
#define EG2_DECAY_TIME_MIN			0.0f
#define EG2_DECAY_TIME_MAX			40.0f
#define EG2_SUSTAIN_LEVEL_DEFAULT	1.0f
#define EG2_SUSTAIN_LEVEL_MIN		0.0f
#define EG2_SUSTAIN_LEVEL_MAX		1.0f
#define EG2_RELEASE_TIME_DEFAULT	0.0f
#define EG2_RELEASE_TIME_MIN		0.0f
#define EG2_RELEASE_TIME_MAX		20.0f
#define EG2_VEL_TO_ATTACK_DEFAULT	0.0f
#define EG2_VEL_TO_ATTACK_MIN		0.0f
#define EG2_VEL_TO_ATTACK_MAX		20.0f
#define EG2_KEY_TO_DECAY_DEFAULT	0.0f
#define EG2_KEY_TO_DECAY_MIN		0.0f
#define EG2_KEY_TO_DECAY_MAX		20.0f
#define EG2_TO_PITCH_DEFAULT		0.0f
#define EG2_TO_PITCH_MIN			-1200.0f
#define EG2_TO_PITCH_MAX			1200.0f
// MISC
#define PAN_DEFAULT					0.0f
#define PAN_MIN						-0.50f
#define PAN_MAX						0.50f

// Struct of basic MIDI events for possible future MIDI input feature
	struct midiEvent {
		byte status;
		byte data1;
		byte data2;
	};

	// TODO: Add other members as needed for other MIDI data i.e. channel number, pitch modulation
	struct noteEvent {
		bool noteOn;
		int channel;
		float note;
		byte midiNote;
		byte velocity;
	};

	class SynthBase {
	public:

		// Time should be in milliseconds
		static UINT32 MsToSamples(float time, UINT32 sampleRate)
		{
			UINT32 SamplesPerMS = sampleRate / 1000;
			UINT32 timePos = (UINT32)time % 1000;
			return timePos * SamplesPerMS;
		}

		static boolean IsNoteOn(midiEvent event)
		{
			return (event.status <= CHANNEL_16_NOTE_ON && event.status >= CHANNEL_1_NOTE_ON);
		}


		static int GetMidiChannel(midiEvent event, boolean noteOn)
		{
			if (noteOn) {
				return (int)event.status - (int)CHANNEL_1_NOTE_ON + 1;
			}
			else {
				return (int)event.status - (int)CHANNEL_1_NOTE_OFF + 1;
			}
		}

		static byte GetChannelOff(byte channelOn)
		{
			return channelOn - byte(0x10);
		}

		// Note num to pitch: (A4_Freq) * 2 ^ ((noteNum - A4_Num) / 12)
		static float MIDIToFreq(byte midiNote)
		{
			return A_4_NOTE * pow(2.0f, ((float)midiNote - (float)MIDI_NOTE_A_4) / 12.0f);
		}

		static noteEvent ConvertEvent(midiEvent event)
		{
			noteEvent nEvent = {
				.noteOn = false,
				.channel = -1,
				.note = 0.0f,
				.midiNote = event.data1,
				.velocity = event.data2
			};

			if (event.status >= CHANNEL_1_NOTE_ON && event.status <= CHANNEL_16_NOTE_ON) {
				nEvent.noteOn = true;
			}
			else {
				nEvent.noteOn = false;
			}
			nEvent.channel = SynthBase::GetMidiChannel(event, nEvent.noteOn);

			if (event.data1 > MIDI_NOTE_G_9)
				nEvent.note = 0.0f;
			else
				nEvent.note = SynthBase::MIDIToFreq(event.data1);

			return nEvent;
		}
	};
}