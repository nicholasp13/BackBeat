#pragma once
#pragma comment(lib, "winmm.lib")

#include "Platform/Windows/Windows.h"
#include "BackBeat/Audio/MIDI/MIDIDevice.h"
namespace BackBeat {

	class WindowsMIDIDevice : public MIDIDevice
	{
	public:
		WindowsMIDIDevice(UINT id, std::string name);
		~WindowsMIDIDevice();

		virtual bool Open() override;
		virtual bool Close() override;
		virtual void Run() override;
		virtual void Stop() override;
		virtual void DeleteOutput(UUID id) override;

		void MIDIInput(DWORD_PTR part1, DWORD_PTR part2);

		inline virtual void PushOutput(std::shared_ptr<MIDIInputHandler> output) override { m_Outputs.push_back(output); }
		inline virtual std::string GetName() override { return m_Name; }
		inline virtual unsigned int GetNumOutputs() override { return (unsigned int)m_Outputs.size(); }
		inline virtual bool IsOpen() override { return m_Open; }

	private:
		bool m_Open;
		bool m_Done;
		UINT m_ID; 
		std::string m_Name;
		std::vector< std::shared_ptr<MIDIInputHandler> > m_Outputs;
		// Windows handler of MIDIDevice filled by midiInOpen()
		HMIDIIN m_DeviceHandle;
	};

}