#pragma once
#pragma comment(lib, "winmm.lib")

// NOTE: Windows API allows System exclusive messaging via buffers. This is not necessary but may be implemented later
#include <mmeapi.h>
#include <mmdeviceapi.h>

#include "MIDIDevice.h"
namespace BackBeat {

	class WindowsMIDIDevice : public MIDIDevice
	{
	public:
		WindowsMIDIDevice(UINT id, std::string name);
		~WindowsMIDIDevice();

		virtual bool Open();
		virtual bool Close();
		virtual void Run();
		virtual void Stop();

		virtual void SetOutput(std::shared_ptr<MIDIInputHandler> output) { m_Output = output; }
		virtual std::string GetName() { return m_Name; }
		virtual bool IsOpen() { return m_Open; }

		void MIDIInput(DWORD_PTR part1, DWORD_PTR part2);

	private:
		bool m_Open;
		bool m_Done;
		UINT m_ID; 
		std::string m_Name;
		
		// Windows handler of MIDIDevice filled by midiInOpen()
		HMIDIIN m_DeviceHandle;

		std::shared_ptr<MIDIInputHandler> m_Output;
	};

}