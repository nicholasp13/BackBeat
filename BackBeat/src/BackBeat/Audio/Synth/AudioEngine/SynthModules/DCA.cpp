#include "bbpch.h"

#include "DCA.h"
namespace BackBeat {

	DCA::DCA(UINT32 bufferSize, std::shared_ptr<float[]> buffer, std::shared_ptr<DCAParameters> params)
		: 
		m_Position(0),
		m_LeftAmp(params->leftAmp),
		m_RightAmp(params->rightAmp),
		m_Params(params),
		m_Buffer(buffer),
		m_ModInput(std::make_unique<Modulator>(bufferSize)),
		m_Type(ModuleType::DCA)
	{
		m_ModInput->FlushBuffer(bufferSize / STEREO, 1.0f);
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

		std::shared_ptr<float[]> modulator = m_ModInput->GetBuffer();

		for (UINT32 i = 0; i < numSamples * STEREO; i++) {
			if (i % 2 == 0) {
				m_Buffer[i] *= m_LeftAmp * modulator[i] / NUM_OSCS;
			}
			else {
				m_Buffer[i] *= m_RightAmp * modulator[i] / NUM_OSCS;
			}
		}

		m_ModInput->FlushBuffer(numSamples, 1.0f);
	}
	
	void DCA::DoNoteOn(NoteEvent event) 
	{

	}
	
	void DCA::DoNoteOff(NoteEvent event)
	{

	}

}