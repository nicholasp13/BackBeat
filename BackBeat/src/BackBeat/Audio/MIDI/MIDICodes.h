#pragma once

// -------- CONSTANTS FOR MIDI EVENTS --------- //
// Website: https://midi.org/specifications/midi-reference-tables/
// TODO: Put these constants into an enum like the Mouse and Key codes
//       Add other constants as needed

namespace BackBeat
{

	using MIDICode = unsigned char;

	namespace MIDI
	{
			constexpr MIDICode
				// ---- MIDI status ---- //
				ChannelOff1 = 0x80,
				ChannelOff2 = 0x81,
				ChannelOff3 = 0x82,
				ChannelOff4 = 0x83,
				ChannelOff5 = 0x84,
				ChannelOff6 = 0x85,
				ChannelOff7 = 0x86,
				ChannelOff8 = 0x87,
				ChannelOff9 = 0x88,
				ChannelOff10 = 0x89,
				ChannelOff11 = 0x8A,
				ChannelOff12 = 0x8B,
				ChannelOff13 = 0x8C,
				ChannelOff14 = 0x8D,
				ChannelOff15 = 0x8E,
				ChannelOff16 = 0x8F,

				ChannelOn1 = 0x90,
				ChannelOn2 = 0x91,
				ChannelOn3 = 0x92,
				ChannelOn4 = 0x93,
				ChannelOn5 = 0x94,
				ChannelOn6 = 0x95,
				ChannelOn7 = 0x96,
				ChannelOn8 = 0x97,
				ChannelOn9 = 0x98,
				ChannelOn10 = 0x99,
				ChannelOn11 = 0x9A,
				ChannelOn12 = 0x9B,
				ChannelOn13 = 0x9C,
				ChannelOn14 = 0x9D,
				ChannelOn15 = 0x9E,
				ChannelOn16 = 0x9F,

				Start = 0xFA,
				Continue = 0xFB,
				Stop = 0xFC,

				// ---- MIDI notes ---- //
				//	-1st octave (0-11)
				CMinus1 = 0x00,  // Lowest note
				CSharpMinus1 = 0x01,
				DMinus1 = 0x02,
				DSharpMinus1 = 0x03,
				EMinus1 = 0x04,
				FMinus1 = 0x05,
				FSharpMinus1 = 0x06,
				GMinus1 = 0x07,
				GSharp_Minus1 = 0x08,
				AMinus1 = 0x09,
				ASharpMinus1 = 0x0A,
				BMinus1 = 0x0B,

				//	0th octave (12-23)
				C0 = 0x0C,
				CSharp0 = 0x0D,
				D0 = 0x0E,
				DSharp0 = 0x0F,
				E0 = 0x10,
				F0 = 0x11,
				FSharp0 = 0x12,
				G0 = 0x13,
				GSharp0 = 0x14,
				A0 = 0x15,
				ASharp0 = 0x16,
				B0 = 0x17,

				//	1st octave (24-35)
				C1 = 0x18,
				CSharp1 = 0x19,
				D1 = 0x1A,
				DSharp_1 = 0x1B,
				E1 = 0x1C,
				F1 = 0x1D,
				FSharp1 = 0x1E,
				G1 = 0x1F,
				GSharp1 = 0x20,
				A1 = 0x21,
				ASharp1 = 0x22,
				B1 = 0x23,

				//	2nd octave (36-47)
				C2 = 0x24,
				CSharp2 = 0x25,
				D2 = 0x26,
				DSharp2 = 0x27,
				E2 = 0x28,
				F2 = 0x29,
				FSharp2 = 0x2A,
				G2 = 0x2B,
				GSharp_2 = 0x2C,
				A2 = 0x2D,
				ASharp_2 = 0x2E,
				B2 = 0x2F,

				//	3rd octave (48-59)
				C3 = 0x30,
				CSharp3 = 0x31,
				D3 = 0x32,
				DSharp_3 = 0x33,
				E3 = 0x34,
				F3 = 0x35,
				FSharp_3 = 0x36,
				G3 = 0x37,
				GSharp3 = 0x38,
				A3 = 0x39,
				ASharp3 = 0x3A,
				B3 = 0x3B,

				//	4th/Middle octave (60-71)
				C4 = 0x3C,
				CSharp4 = 0x3D,
				D4 = 0x3E,
				DSharp4 = 0x3F,
				E4 = 0x40,
				F4 = 0x41,
				FSharp4 = 0x42,
				G4 = 0x43,
				GSharp4 = 0x44,
				A4 = 0x45,
				ASharp4 = 0x46,
				B4 = 0x47,

				//	5th octave (72-83)
				C5 = 0x48,
				CSharp5 = 0x49,
				D5 = 0x4A,
				DSharp5 = 0x4B,
				E5 = 0x4C,
				F5 = 0x4D,
				FSharp5 = 0x4E,
				G5 = 0x4F,
				GSharp5 = 0x50,
				A5 = 0x51,
				ASharp5 = 0x52,
				B5 = 0x53,

				//	6th octave (84-95)
				C6 = 0x54,
				CSharp6 = 0x55,
				D6 = 0x56,
				DSharp6 = 0x57,
				E6 = 0x58,
				F6 = 0x59,
				FSharp6 = 0x5A,
				G6 = 0x5B,
				GSharp6 = 0x5C,
				A6 = 0x5D,
				ASharp6 = 0x5E,
				B6 = 0x5F,

				//	7th octave (96-107)
				C7 = 0x60,
				CSharp7 = 0x61,
				D7 = 0x62,
				DSharp7 = 0x63,
				E7 = 0x64,
				F7 = 0x65,
				FSharp7 = 0x66,
				G7 = 0x67,
				GSharp7 = 0x68,
				A_7 = 0x69,
				Aharp_7 = 0x6A,
				B7 = 0x6B,

				//	8th octave (108-119)
				C8 = 0x6C,
				CSharp8 = 0x6D,
				D8 = 0x6E,
				DSharp8 = 0x6F,
				E8 = 0x70,
				F8 = 0x71,
				FSharp8 = 0x72,
				G8 = 0x73,
				GSharp8 = 0x74,
				A8 = 0x75,
				ASharp8 = 0x76,
				B8 = 0x77,

				//	9th octave (120-127)
				C9 = 0x78,
				CSharp_9 = 0x79,
				D9 = 0x7A,
				DSharp9 = 0x7B,
				E9 = 0x7C,
				F9 = 0x7D,
				FSharp9 = 0x7E,
				G9 = 0x7F,      // Highest note

				// ---- MIDI misc. ----
				NumMIDINotes = 0x80,
				NoteOff = 0xFF,
				MinVelocity = 0x00,
				MaxVelocity = 0x7F;

			constexpr const char* MIDINoteNames[NumMIDINotes] =
			{
				"C-1", "C#-1/Db-1", "D-1", "D#-1/Eb-1", "E-1", "F-1", "F#-1/Gb-1", "G-1", "G#-1/Ab-1", "A-1", "A#-1/Bb-1", "B-1",
				"C0", "C#0/Db0", "D0", "D#0/Eb0", "E0", "F0", "F#0/Gb0", "G0", "G#0/Ab0", "A0", "A#0/Bb0", "B0",
				"C1", "C#1/Db1", "D1", "D#1/Eb1", "E1", "F1", "F#1/Gb1", "G1", "G#1/Ab1", "A1", "A#1/Bb1", "B1",
				"C2", "C#2/Db2", "D2", "D#2/Eb2", "E2", "F2", "F#2/Gb2", "G2", "G#2/Ab2", "A2", "A#2/Bb2", "B2",
				"C3", "C#3/Db3", "D3", "D#3/Eb3", "E3", "F3", "F#3/Gb3", "G3", "G#3/Ab3", "A3", "A#3/Bb3", "B3",
				"C4", "C#4/Db4", "D4", "D#4/Eb4", "E4", "F4", "F#4/Gb4", "G4", "G#4/Ab4", "A4", "A#4/Bb4", "B4",
				"C5", "C#5/Db5", "D5", "D#5/Eb5", "E5", "F5", "F#5/Gb5", "G5", "G#5/Ab5", "A5", "A#5/Bb5", "B5",
				"C6", "C#6/Db6", "D6", "D#6/Eb6", "E6", "F6", "F#6/Gb6", "G6", "G#6/Ab6", "A6", "A#6/Bb6", "B6",
				"C7", "C#7/Db7", "D7", "D#7/Eb7", "E7", "F7", "F#7/Gb7", "G7", "G#7/Ab7", "A7", "A#7/Bb7", "B7",
				"C8", "C#8/Db8", "D8", "D#8/Eb8", "E8", "F8", "F#8/Gb8", "G8", "G#8/Ab8", "A8", "A#8/Bb8", "B8",
				"C9", "C#9/Db9", "D9", "D#9/Eb9", "E9", "F9", "F#9/Gb9", "G9"
			};
	}
}