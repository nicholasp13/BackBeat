#include "bbpch.h"

#include "WindowsMIDIDeviceManager.h"
namespace BackBeat {

	WindowsMIDIDeviceManager::WindowsMIDIDeviceManager()
		:
		m_OutputSet(false),
		m_NumDevices(0),
		m_RunID(-1)
	{
		InitDevices();
	}

	WindowsMIDIDeviceManager::~WindowsMIDIDeviceManager()
	{
		if (m_RunID >= 0)
			StopDevice();
	}

	bool WindowsMIDIDeviceManager::OpenDevice(UINT index)
	{
		if (!m_OutputSet) {
			BB_CORE_ERROR("SET MIDI DEVICE OUTPUT");
			return false;
		}

		if (!m_Devices[index].IsOpen())
			return m_Devices[index].Open();
		return true;
	}

	bool WindowsMIDIDeviceManager::CloseDevice(UINT index)
	{
		if (!m_OutputSet) {
			BB_CORE_ERROR("SET MIDI DEVICE OUTPUT");
			return false;
		}

		if (index == m_RunID)
			StopDevice();
		return m_Devices[index].Close();
	}

	bool WindowsMIDIDeviceManager::RunDevice(UINT index)
	{
		if (!m_OutputSet) {
			BB_CORE_ERROR("SET MIDI DEVICE OUTPUT");
			return false;
		}

		if (m_RunID < 0) {
			m_Devices[index].Run();
			m_RunID = index;
			return true;
		} 
		else {
			m_Devices[m_RunID].Stop();
			m_Devices[index].Run();
			m_RunID = index;
			return true;
		}

		return false;
	}

	bool WindowsMIDIDeviceManager::StopDevice()
	{
		m_Devices[m_RunID].Stop();
		m_Devices[m_RunID].Close();
		m_RunID = -1;
		return true;
	}

	bool WindowsMIDIDeviceManager::CloseAll()
	{
		if (m_RunID >= 0)
			m_Devices[m_RunID].Stop();
		for (UINT i = 0; i < m_NumDevices; i++) {
			m_Devices[i].Close();
		}
		return true;
	}

	void WindowsMIDIDeviceManager::PushOutput(std::shared_ptr<MIDIInputHandler> output)
	{
		for (UINT i = 0; i < m_NumDevices; i++) {
			m_Devices[i].PushOutput(output);
		}
		m_OutputSet = true;
	}

	void WindowsMIDIDeviceManager::InitDevices()
	{
		MMRESULT mr = MMSYSERR_NOERROR;
		m_NumDevices = midiInGetNumDevs();
		MIDIINCAPSA deviceInfo;
		std::string name;

		for (UINT i = 0; i < m_NumDevices; i++) {
			deviceInfo = MIDIINCAPSA();

			// MMRESULT midiInGetDevCapsA(
			//  UINT         uDeviceID,    // ID of MIDI device. From 0 to less than num devices. Can also be a handle
			//  LPMIDIINCAPS pmic,         // Pointer to MIDIINCAPS
			//  UINT         cbmic         // Size in bytes of MIDIINCAPS
			// );
			// NOTE: Two different functions with the same name except the last letter is either 'A' or 'W'
			//       - A - Is regular char for the device name
			//       - W - Is wChar for the device name
			mr = midiInGetDevCapsA(i, (LPMIDIINCAPSA)(&deviceInfo), sizeof(deviceInfo));
			// CHECK_FAILURE(mr);

			name = reinterpret_cast<char*>(deviceInfo.szPname);
			m_DeviceInfo.push_back(deviceInfo);
			m_Devices.push_back(WindowsMIDIDevice(i, name));
		}
	}

}