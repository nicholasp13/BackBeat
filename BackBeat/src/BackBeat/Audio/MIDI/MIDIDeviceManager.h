#pragma once

#include "bbpch.h"

// TODO: Add multiple outputs

#include "MIDIDevice.h"
namespace BackBeat {

	class MIDIDeviceManager
	{
	public:
		// NOTE: Might not use bool return values
		virtual bool OpenDevice(unsigned int index) { return false; };
		virtual bool CloseDevice(unsigned int index) { return false; };
		virtual bool RunDevice(unsigned int index) { return false; };
		// Note: No index required as there is assumed only one device running. However, that may change
		virtual bool StopDevice() { return false; };
		virtual bool CloseAll() { return false; };

		virtual std::string GetDeviceName(unsigned int index) { return std::string(); };
		virtual unsigned int GetNumDevices() { return 0; };
		virtual bool IsRunning() { return false; };
		virtual bool IsOpen(unsigned int index) { return false; };
		virtual void SetOutput(std::shared_ptr<MIDIInputHandler> output) {};
	};

}