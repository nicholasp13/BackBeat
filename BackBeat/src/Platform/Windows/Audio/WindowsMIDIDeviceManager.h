#pragma once

// TODO: Add ability to check when devices are plugged and unplugged 
//       (Use Windows IMMDeviceEnumerator::EnumAudioEndpoints method)

#include "WindowsMIDIDevice.h"
#include "BackBeat/Audio/MIDI/MIDIDeviceManager.h"
namespace BackBeat {

	// NOTE: May implment parameters to allow for GUI input
	//       Indeces range from 0 to (m_NumDevices - 1).
	class WindowsMIDIDeviceManager : public MIDIDeviceManager
	{
	public:
		WindowsMIDIDeviceManager();
		~WindowsMIDIDeviceManager();

		virtual bool OpenDevice(UINT index) override;
		virtual bool CloseDevice(UINT index) override;
		virtual bool RunDevice(UINT index) override;
		virtual bool StopDevice() override;
		virtual bool CloseAll() override;
		virtual void PushOutput(std::shared_ptr<MIDIInputHandler> output) override;
		virtual void DeleteOutput(UUID id) override;

		// NOTE: Getters for other MIDI device data not implemented
		inline virtual std::string GetDeviceName(UINT index) override { return m_Devices[index].GetName(); }
		inline virtual UINT GetNumDevices() override { return m_NumDevices; }
		inline virtual bool IsOpen(UINT index) override { return m_Devices[index].IsOpen(); };
		inline virtual bool IsRunning(UINT index) override { return (m_RunID >= (int)index); }

	private:
		unsigned int m_NumOutputsSet;
		UINT m_NumDevices;
		// Set to -1 if no devices are running
		int m_RunID;

		std::thread m_Thread;
		// Vector of Windows struct
		// typedef struct {   
 		// WORD      wMid;                    // Manufacturer ID of MIDIDevice, list of possible IDs not kept by Microsoft
		// WORD      wPid;                    // Product ID of MIDIDevice, list of possible IDs not kept by Microsoft
		// MMVERSION vDriverVersion;          // Version number of the device driver of the MIDIDevice - high-order byte: major number, low-order byte: minor number
		// TCHAR     szPname[MAXPNAMELEN];    // Name given by Windows char array that is null-terminated
		// DWORD     dwSupport;               // Reserved, must be 0.
		// } MIDIINCAPS;
		std::vector<MIDIINCAPSA> m_DeviceInfo;
		std::vector<WindowsMIDIDevice> m_Devices;
		std::shared_ptr<MIDIInputHandler> m_Output;

	private:
		void InitDevices();
	};
}