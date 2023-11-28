#pragma once

#include "bbpch.h"

#include "MIDIDevice.h"
namespace BackBeat {

	class MIDIDeviceManager
	{
	public:
		// NOTE: Might not use bool return values
		virtual bool OpenDevice(UINT index) { return false; };
		virtual bool CloseDevice(UINT index) { return false; };
		virtual bool RunDevice(UINT index) { return false; };
		// Note: No index required as there is assumed only one device running. However, that may change
		virtual bool StopDevice() { return false; };
		virtual bool CloseAll() { return false; };

		virtual std::string GetDeviceName(UINT index) { return std::string(); };
		virtual UINT GetNumDevices() { return 0; };
		virtual bool IsRunning() { return false; };
		virtual bool IsOpen(UINT index) { return false; };
		virtual void SetOutput(std::shared_ptr<MIDIInputHandler> output) {};
	};

}