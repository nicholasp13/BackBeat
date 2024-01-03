#pragma once

// -------- CONSTANTS FOR MIDI EVENTS --------- //
// Website: https://midi.org/specifications/midi-reference-tables/
// TODO: Put these constants into an enum like the Mouse and Key codes
//       Add other constants as needed

namespace BackBeat
{

	namespace MIDI
	{
		// ---- MIDI status ---- //
		constexpr unsigned char ChannelOff_1 = 0x80;
		constexpr unsigned char ChannelOff_2 = 0x81;
		constexpr unsigned char ChannelOff_3 = 0x82;
		constexpr unsigned char ChannelOff_4 = 0x83;
		constexpr unsigned char ChannelOff_5 = 0x84;
		constexpr unsigned char ChannelOff_6 = 0x85;
		constexpr unsigned char ChannelOff_7 = 0x86;
		constexpr unsigned char ChannelOff_8 = 0x87;
		constexpr unsigned char ChannelOff_9 = 0x88;
		constexpr unsigned char ChannelOff_10 = 0x89;
		constexpr unsigned char ChannelOff_11 = 0x8A;
		constexpr unsigned char ChannelOff_12 = 0x8B;
		constexpr unsigned char ChannelOff_13 = 0x8C;
		constexpr unsigned char ChannelOff_14 = 0x8D;
		constexpr unsigned char ChannelOff_15 = 0x8E;
		constexpr unsigned char ChannelOff_16 = 0x8F;

		constexpr unsigned char ChannelOn_1 = 0x90;
		constexpr unsigned char ChannelOn_2 = 0x91;
		constexpr unsigned char ChannelOn_3 = 0x92;
		constexpr unsigned char ChannelOn_4 = 0x93;
		constexpr unsigned char ChannelOn_5 = 0x94;
		constexpr unsigned char ChannelOn_6 = 0x95;
		constexpr unsigned char ChannelOn_7 = 0x96;
		constexpr unsigned char ChannelOn_8 = 0x97;
		constexpr unsigned char ChannelOn_9 = 0x98;
		constexpr unsigned char ChannelOn_10 = 0x99;
		constexpr unsigned char ChannelOn_11 = 0x9A;
		constexpr unsigned char ChannelOn_12 = 0x9B;
		constexpr unsigned char ChannelOn_13 = 0x9C;
		constexpr unsigned char ChannelOn_14 = 0x9D;
		constexpr unsigned char ChannelOn_15 = 0x9E;
		constexpr unsigned char ChannelOn_16 = 0x9F;

		constexpr unsigned char Start = 0xFA;
		constexpr unsigned char Continue = 0xFB;
		constexpr unsigned char Stop = 0xFC;

		// ---- MIDI notes ---- //
		//	-1st octave (0-11)
		constexpr unsigned char C_Minus1 = 0x00;  // Lowest note
		constexpr unsigned char CSharp_Minus1 = 0x01;
		constexpr unsigned char D_Minus1 = 0x02;
		constexpr unsigned char DSharp_Minus1 = 0x03;
		constexpr unsigned char E_Minus1 = 0x04;
		constexpr unsigned char F_Minus1 = 0x05;
		constexpr unsigned char FSharp_Minus1 = 0x06;
		constexpr unsigned char G_Minus1 = 0x07;
		constexpr unsigned char GSharp_Minus1 = 0x08;
		constexpr unsigned char A_Minus1 = 0x09;
		constexpr unsigned char ASharp_Minus1 = 0x0A;
		constexpr unsigned char B_Minus1 = 0x0B;
			
		//	0th octave (12-23)
		constexpr unsigned char C_0 = 0x0C;
		constexpr unsigned char CSharp_0 = 0x0D;
		constexpr unsigned char D_0 = 0x0E;
		constexpr unsigned char DSharp_0 = 0x0F;
		constexpr unsigned char E_0 = 0x10;
		constexpr unsigned char F_0 = 0x11;
		constexpr unsigned char FSharp_0 = 0x12;
		constexpr unsigned char G_0 = 0x13;
		constexpr unsigned char GSharp_0 = 0x14;
		constexpr unsigned char A_0 = 0x15;
		constexpr unsigned char ASharp_0 = 0x16;
		constexpr unsigned char B_0 = 0x17;
			
		//	1st octave (24-35)
		constexpr unsigned char C_1 = 0x18;
		constexpr unsigned char CSharp_1 = 0x19;
		constexpr unsigned char D_1 = 0x1A;
		constexpr unsigned char DSharp_1 = 0x1B;
		constexpr unsigned char E_1 = 0x1C;
		constexpr unsigned char F_1 = 0x1D;
		constexpr unsigned char FSharp_1 = 0x1E;
		constexpr unsigned char G_1 = 0x1F;
		constexpr unsigned char GSharp_1 = 0x20;
		constexpr unsigned char A_1 = 0x21;
		constexpr unsigned char ASharp_1 = 0x22;
		constexpr unsigned char B_1 = 0x23;

		//	2nd octave (36-47)
		constexpr unsigned char C_2 = 0x24;
		constexpr unsigned char CSharp_2 = 0x25;
		constexpr unsigned char D_2 = 0x26;
		constexpr unsigned char DSharp_2 = 0x27;
		constexpr unsigned char E_2 = 0x28;
		constexpr unsigned char F_2 = 0x29;
		constexpr unsigned char FSharp_2 = 0x2A;
		constexpr unsigned char G_2 = 0x2B;
		constexpr unsigned char GSharp_2 = 0x2C;
		constexpr unsigned char A_2 = 0x2D;
		constexpr unsigned char ASharp_2 = 0x2E;
		constexpr unsigned char B_2 = 0x2F;

		//	3rd octave (48-59)
		constexpr unsigned char C_3 = 0x30;
		constexpr unsigned char CSharp_3 = 0x31;
		constexpr unsigned char D_3 = 0x32;
		constexpr unsigned char DSharp_3 = 0x33;
		constexpr unsigned char E_3 = 0x34;
		constexpr unsigned char F_3 = 0x35;
		constexpr unsigned char FSharp_3 = 0x36;
		constexpr unsigned char G_3 = 0x37;
		constexpr unsigned char GSharp_3 = 0x38;
		constexpr unsigned char A_3 = 0x39;
		constexpr unsigned char ASharp_3 = 0x3A;
		constexpr unsigned char B_3 = 0x3B;

		//	4th/Middle octave (60-71)
		constexpr unsigned char C_4 = 0x3C;
		constexpr unsigned char CSharp_4 = 0x3D;
		constexpr unsigned char D_4 = 0x3E;
		constexpr unsigned char DSharp_4 = 0x3F;
		constexpr unsigned char E_4 = 0x40;
		constexpr unsigned char F_4 = 0x41;
		constexpr unsigned char FSharp_4 = 0x42;
		constexpr unsigned char G_4 = 0x43;
		constexpr unsigned char GSharp_4 = 0x44;
		constexpr unsigned char A_4 = 0x45;
		constexpr unsigned char ASharp_4 = 0x46;
		constexpr unsigned char B_4 = 0x47;

		//	5th octave (72-83)
		constexpr unsigned char C_5 = 0x48;
		constexpr unsigned char CSharp_5 = 0x49;
		constexpr unsigned char D_5 = 0x4A;
		constexpr unsigned char DSharp_5 = 0x4B;
		constexpr unsigned char E_5 = 0x4C;
		constexpr unsigned char F_5 = 0x4D;
		constexpr unsigned char FSharp_5 = 0x4E;
		constexpr unsigned char G_5 = 0x4F;
		constexpr unsigned char GSharp_5 = 0x50;
		constexpr unsigned char A_5 = 0x51;
		constexpr unsigned char ASharp_5 = 0x52;
		constexpr unsigned char B_5 = 0x53;

		//	6th octave (84-95)
		constexpr unsigned char C_6 = 0x54;
		constexpr unsigned char CSharp_6 = 0x55;
		constexpr unsigned char D_6 = 0x56;
		constexpr unsigned char DSharp_6 = 0x57;
		constexpr unsigned char E_6 = 0x58;
		constexpr unsigned char F_6 = 0x59;
		constexpr unsigned char FSharp_6 = 0x5A;
		constexpr unsigned char G_6 = 0x5B;
		constexpr unsigned char GSharp_6 = 0x5C;
		constexpr unsigned char A_6 = 0x5D;
		constexpr unsigned char ASharp_6 = 0x5E;
		constexpr unsigned char B_6 = 0x5F;

		//	7th octave (96-107)
		constexpr unsigned char C_7 = 0x60;
		constexpr unsigned char CSharp_7 = 0x61;
		constexpr unsigned char D_7 = 0x62;
		constexpr unsigned char DSharp_7 = 0x63;
		constexpr unsigned char E_7 = 0x64;
		constexpr unsigned char F_7 = 0x65;
		constexpr unsigned char FSharp_7 = 0x66;
		constexpr unsigned char G_7 = 0x67;
		constexpr unsigned char GSharp_7 = 0x68;
		constexpr unsigned char A_7 = 0x69;
		constexpr unsigned char ASharp_7 = 0x6A;
		constexpr unsigned char B_7 = 0x6B;

		//	8th octave (108-119)
		constexpr unsigned char C_8 = 0x6C;
		constexpr unsigned char CSharp_8 = 0x6D;
		constexpr unsigned char D_8 = 0x6E;
		constexpr unsigned char DSharp_8 = 0x6F;
		constexpr unsigned char E_8 = 0x70;
		constexpr unsigned char F_8 = 0x71;
		constexpr unsigned char FSharp_8 = 0x72;
		constexpr unsigned char G_8 = 0x73;
		constexpr unsigned char GSharp_8 = 0x74;
		constexpr unsigned char A_8 = 0x75;
		constexpr unsigned char ASharp_8 = 0x76;
		constexpr unsigned char B_8 = 0x77;

		//	9th octave (120-127)
		constexpr unsigned char C_9 = 0x78;
		constexpr unsigned char CSharp_9 = 0x79;
		constexpr unsigned char D_9 = 0x7A;
		constexpr unsigned char DSharp_9 = 0x7B;
		constexpr unsigned char E_9 = 0x7C;
		constexpr unsigned char F_9 = 0x7D;
		constexpr unsigned char FSharp_9 = 0x7E;
		constexpr unsigned char G_9 = 0x7F;      // Highest note

		// ---- MIDI misc. ----
		constexpr unsigned char NoteOff  = 0xFF;
		constexpr unsigned char MinVelocity = 0x00;
		constexpr unsigned char MaxVelocity = 0x7F;
	}
}