#pragma once

#include "bbpch.h"

#include "BackBeat/Audio/Audio.h"
#include "BackBeat/Audio/Synth/SynthIO/MIDIInputHandler.h"
namespace BackBeat {
	
	// NOTE: Might need enum types and/or subclasses to specify if the device is input or output 
	class MIDIDevice
	{
	public:
		virtual bool Open() { return false; };
		virtual bool Close() { return false; };
		virtual void Run() {};
		virtual void Stop() {};

		virtual void SetOutput(std::shared_ptr<MIDIInputHandler> output) {};
		virtual std::string GetName() { return std::string(); };
		virtual bool IsOpen() { return false; };
		
	};
}