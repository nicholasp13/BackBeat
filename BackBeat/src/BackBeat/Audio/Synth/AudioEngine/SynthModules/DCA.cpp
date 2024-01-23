#include "bbpch.h"

#include "DCA.h"
namespace BackBeat {

	DCA::DCA(unsigned int bufferSize, std::shared_ptr<float[]> buffer, std::shared_ptr<DCAParameters> params)
		: 
		m_Position(0),
		m_LeftAmp(params->leftAmp),
		m_RightAmp(params->rightAmp),
		m_Volume(1.0f),
		m_Params(params),
		m_Buffer(buffer),
		m_ModInput(std::make_unique<Modulator>(bufferSize)),
		m_Type(ModuleType::DCA)
	{
		m_ModInput->FlushBuffer(bufferSize / Audio::Stereo, 1.0f);
	}

	DCA::~DCA()
	{

	}

	void DCA::Reset(unsigned int sampleRate)
	{
		m_Position = 0;
	}

	void DCA::Update() 
	{
		m_LeftAmp = m_Params->leftAmp;
		m_RightAmp = m_Params->rightAmp;
		m_Volume = m_Params->volume;
	}
	
	void DCA::Render(unsigned int numSamples)
	{
		Update();

		std::shared_ptr<float[]> modulator = m_ModInput->GetBuffer();

		for (unsigned int i = 0; i < numSamples * Audio::Stereo; i++) {
			if (i % 2 == 0) 
			{
				m_Buffer[i] *= m_LeftAmp * m_Volume * modulator[i] / SynthBase::NumOscillators;
			}
			else 
			{
				m_Buffer[i] *= m_RightAmp * m_Volume * modulator[i] / SynthBase::NumOscillators;
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