#include "bbpch.h"

#include "DCA.h"
namespace BackBeat {

	DCA::DCA(UINT32 sampleRate, UINT32 bufferSize, std::shared_ptr<float[]> buffer)
		: 
		m_SampleRate(sampleRate), 
		m_BufferSize(bufferSize),
		m_Buffer(buffer),
		m_Position(0),
		m_RightAmp(1.0f),
		m_LeftAmp(1.0f),
		m_Type(ModuleType::DCA)
	{

	}

	DCA::~DCA()
	{

	}

	void DCA::Reset(UINT32 sampleRate)
	{
		m_SampleRate = sampleRate;
		m_Position = 0;
	}

	// TODO: Implement after creating ModulationMatrix object to allow for dynamic change
	void DCA::Update() 
	{
		
	}
	
	void DCA::Render(UINT32 numSamples) 
	{
		Update();

		for (UINT32 i = 0; i < numSamples * 2; i++) {
			if (i % 2 == 0) {
				m_Buffer[i] *= m_LeftAmp;
			}
			else {
				m_Buffer[i] *= m_RightAmp;
			}
		}
	}
	
	void DCA::DoNoteOn(noteEvent event) 
	{

	}
	
	void DCA::DoNoteOff(noteEvent event)
	{

	}

}