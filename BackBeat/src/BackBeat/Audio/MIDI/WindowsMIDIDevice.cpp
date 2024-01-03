#include "bbpch.h"

#include "MMSystem.h"

// TODO: Create function/macro for MMERROR
//       Create function for MIDI errors
#include "BackBeat/Core/Timer.h"
#include "WindowsMIDIDevice.h"
namespace BackBeat {

	// Windows callback function that is called everytime there is a MIDI input
	// Note: Not a WindowsMIDIDevice function
	void CALLBACK MidiInProc(
		HMIDIIN hMidiIn,
		UINT msg,            // MIDI input message type
		DWORD_PTR instance,  // User data
		DWORD_PTR param1,    // MIDI input bytes
		DWORD_PTR param2     // MIDI input bytes
	) 
	{
		WindowsMIDIDevice* device = reinterpret_cast<WindowsMIDIDevice*>(instance);

		switch (msg)
		{

		case (MIM_CLOSE):
		{
			break;
		}

		case (MIM_DATA):
		{
			device->MIDIInput(param1, param2);
			break;
		}

		case (MIM_ERROR):
		{
			// When error occurs from the MIDI input device
			// param1 = dwMidiMessage - Error message in two bytes, High: 2nd part, Low: 1st part
			// param2 = dwTimestamp   - Time error occured from midiInStart()
			break;
		}

		// NOTE: System exclusive messages not implemented currently
		case (MIM_LONGDATA):
		{
			// System exclusive message that is processed in given buffer by midiInAddBuffer() by instance
			// param1 = (DWORD) lpMidiHdr - Header to the input buffer with the message
			// param2 = dwTimestamp
			break;
		}
		case (MIM_LONGERROR):
		{
			// Incomplete or invalid system-exclusive method
			// param1 = (DWORD) lpMidiHdr - Header to the input buffer with the error message
			// param2 = dwTimestamp
			break;
		}

		case (MIM_MOREDATA):
		{
			// Application is not processing MIDI data fast enough
			// NOTE: Only is called when app specifies MIDI_IO_STATUS in the call to the midiInOpen()
			//       Params same as MIM_DATA
			device->MIDIInput(param1, param2);
			break;
		}

		case (MIM_OPEN):
		{
			break;
		}

		default:
		{
			break;
		}

		}
	};

	WindowsMIDIDevice::WindowsMIDIDevice(UINT id, std::string name)
		:
		m_Open(false),
		m_Done(false),
		m_ID(id),
		m_Name(name),
		m_DeviceHandle(HMIDIIN())
	{

	}

	WindowsMIDIDevice::~WindowsMIDIDevice()
	{
		if (m_Open)
			Close();
	}

	bool WindowsMIDIDevice::Open()
	{
		MMRESULT mr = MMSYSERR_NOERROR;

		// midiInOpen(
		//  LPHMIDIIN lphMidiIn,          // Pointer to the handle of MIDI input device, used in other MIDI input functions
		//  UINT      uDeviceID,          // ID to the MIDI input device 
		//	DWORD_PTR dwCallback,         // Points to the user's callback function
		//	DWORD_PTR dwCallbackInstance, // User data passed to the callback function
		//	DWORD     dwFlags             // Callback flags
		//	);
		// Returns MMSYSERR_NOERROR if successful or MMRESULT error message if not
		// Flags used in dwFlags:
		//     - CALLBACK_FUNCTION - The dwCallback parameter is a callback procedure address
		//     - CALLBACK_NULL     - There is no callback function, default flag
		//     - CALLBACK_THREAD   - The dwCallback is a thread identifier
		//     - CALLBACK_WINDOW   - the dwCallback is a window handle
		mr = midiInOpen(
				(LPHMIDIIN)(&m_DeviceHandle),
				m_ID,
				reinterpret_cast<DWORD_PTR>(BackBeat::MidiInProc),
				reinterpret_cast<DWORD_PTR>(this),
				CALLBACK_FUNCTION
		);
		// CHECK_FAILURE(mr);

		m_Open = true;
		return m_Open;
	}

	bool WindowsMIDIDevice::Close()
	{
		MMRESULT mr = MMSYSERR_NOERROR;

		// NOTE: If using midiInAddBuffer(), return all buffers before calling midiInClose() (not used)
		mr = midiInClose(m_DeviceHandle);
		// CHECK_FAILURE(mr);

		m_Open = false;
		return m_Open;
	}

	void WindowsMIDIDevice::Run()
	{
		if (!m_Open)
			return;
		MMRESULT mr = midiInStart(m_DeviceHandle);
		// CHECK_FAILURE(mr);
	}

	void WindowsMIDIDevice::Stop()
	{
		MMRESULT mr = midiInStop(m_DeviceHandle);
		// CHECK_FAILURE(mr);
	}

	void WindowsMIDIDevice::MIDIInput(DWORD_PTR part1, DWORD_PTR part2)
	{
		MIDIEvent event = {
			.status = 0x00,
			.data1 = 0x00,
			.data2 = 0x00,
		};

		// NOTE: Logging comments still here for testing MIDI inputs
		// byte a1 = (byte)((part1 & 0x000000FF));       // status
		// byte a2 = (byte)((part1 & 0x0000FF00) >> BYTE_BIT_SIZE);  // data1
		// byte a3 = (byte)((part1 & 0x00FF0000) >> BYTE_BIT_SIZE * 2); // data2
		// byte a4 = (byte)((part1 & 0xFF000000) >> BYTE_BIT_SIZE * 3); // <--- BYTE NOT USED
		// BB_CORE_INFO("BYTES FOR PART 1: {4}\n1: {0} \n2: {1}\n3: {2}\n4: {3}", a1, a2, a3, a4, part1);


		// Time stamps, not implemented
		// byte b1 = (byte)((part2 & 0x000000FF));       // Most
		// byte b2 = (byte)((part2 & 0x0000FF00) >> BYTE_BIT_SIZE);  // More
		// byte b3 = (byte)((part2 & 0x00FF0000) >> BYTE_BIT_SIZE * 2); // Lesser
		// byte b4 = (byte)((part2 & 0xFF000000) >> BYTE_BIT_SIZE * 3); // Least

		// BB_CORE_INFO("BYTES FOR PART 2: {4}\n1: {0} \n2: {1}\n3: {2}\n4: {3}", b1, b2, b3, b4, part2);
		
		// This byte is either CHANNEL_1_NOTE_ON or CHANNEL_1_NOTE_OFF | NOTE: This might change with multiple MIDI devices plugged in
		event.status = (byte)((part1 & 0x000000FF));
		event.data1 =  (byte)((part1 & 0x0000FF00) >> Audio::ByteBitSize);
		event.data2 = (byte)((part1 & 0x00FF0000) >> Audio::ByteBitSize * 2);
		
		m_Output->In(event);
	}

}