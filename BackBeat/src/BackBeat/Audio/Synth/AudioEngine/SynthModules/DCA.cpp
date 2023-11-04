#include "bbpch.h"

#include "DCA.h"
namespace BackBeat {

	DCA::DCA(std::shared_ptr<float[]> buffer, std::shared_ptr<DCAParameters> params)
		: 
		m_Position(0),
		m_LeftAmp(params->leftAmp),
		m_RightAmp(params->rightAmp),
		m_Params(params),
		m_Buffer(buffer),
		m_Type(ModuleType::DCA)
	{

	}

	DCA::~DCA()
	{

	}

	void DCA::Reset(UINT32 sampleRate)
	{
		m_Position = 0;
	}

	void DCA::Update() 
	{
		m_LeftAmp = m_Params->leftAmp;
		m_RightAmp = m_Params->rightAmp;
	}
	
	void DCA::Render(UINT32 numSamples) 
	{
		Update();

		for (UINT32 i = 0; i < numSamples * STEREO; i++) {
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