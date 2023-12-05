#include "bbpch.h"

#include "Mixer.h"
namespace BackBeat {

	Mixer::Mixer(AudioProps props)
		: m_NumProcs(0), m_Props(props), m_Procs(std::vector< std::shared_ptr<AudioProcessor> >())
	{

	}

	Mixer::~Mixer()
	{

	}

	// NOTE: Upsampling, downsampling, and adjusting for number of channels will be handled by AudioProcessors
	//       This only handles adjusting for different bitdepths
	void Mixer::GetData(byte* data, unsigned int numSamples)
	{
		for (unsigned int i = 0; i < m_Procs.size(); i++) {
			if (!m_Procs[i]->IsOn())
				continue;
			m_Procs[i]->ProcessSamples(numSamples, m_Props.sampleRate, m_Props.numChannels); // TODO: Multithread this call
		}
		// Flush data buffer
		for (unsigned int k = 0; k < numSamples * m_Props.blockAlign; k ++)
		{
			data[k] = (byte)0x00;
		}

		float depthRatio = 0.0f;

		switch (m_Props.bitDepth)
		{

		case (BYTE_BIT_SIZE):
		{
			return;
		}

		case (INT16_BIT_SIZE):
		{
			return;
		}

		case (INT24_BIT_SIZE):
		{
			return;
		}

		case (FLOAT_BIT_SIZE):
		{
			float* targetBuffer = reinterpret_cast<float*>(data);
			for (unsigned int i = 0; i < m_Procs.size(); i++) {
				if (!m_Procs[i]->IsOn())
					continue;

				AudioProps inProps = m_Procs[i]->GetProperties();
				depthRatio = GetTypeRatio(m_Props.bitDepth, inProps.bitDepth);
				
				if (inProps.bitDepth == BYTE_BIT_SIZE) {
					auto srcBuffer = reinterpret_cast<byte*>(m_Procs[i]->GetOutputBuffer());

					for (unsigned int i = 0; i < numSamples * m_Props.numChannels; i += m_Props.numChannels)
					{
						for (unsigned int j = 0; j < m_Props.numChannels; j++)
						{
							targetBuffer[i + j] += (float)srcBuffer[i + j] * depthRatio;
						}
					}
				}
				else if (inProps.bitDepth == INT16_BIT_SIZE) {
					auto srcBuffer = reinterpret_cast<signed short*>(m_Procs[i]->GetOutputBuffer());

					for (unsigned int i = 0; i < numSamples * m_Props.numChannels; i += m_Props.numChannels)
					{
						for (unsigned int j = 0; j < m_Props.numChannels; j++)
						{
							targetBuffer[i + j] += (float)srcBuffer[i + j] * depthRatio;
						}
					}
				}

				else if (inProps.bitDepth == INT24_BIT_SIZE) {
					
					auto srcBuffer = reinterpret_cast<signed long*>(m_Procs[i]->GetOutputBuffer());

					for (unsigned int i = 0; i < numSamples * m_Props.numChannels; i += m_Props.numChannels)
					{
						for (unsigned int j = 0; j < m_Props.numChannels; j++)
						{
							targetBuffer[i + j] += (float)srcBuffer[i + j] * depthRatio;
						}
					}

				}

				else if (inProps.bitDepth == FLOAT_BIT_SIZE) {
					auto srcBuffer = reinterpret_cast<float*>(m_Procs[i]->GetOutputBuffer());

					for (unsigned int i = 0; i < numSamples * m_Props.numChannels; i += m_Props.numChannels)
					{
						for (unsigned int j = 0; j < m_Props.numChannels; j++)
						{
							targetBuffer[i + j] += (float)srcBuffer[i + j] * depthRatio;
						}
					}
				}

				else if (inProps.bitDepth == DOUBLE_BIT_SIZE) {
					auto srcBuffer = reinterpret_cast<double*>(m_Procs[i]->GetOutputBuffer());

					for (unsigned int i = 0; i < numSamples * m_Props.numChannels; i += m_Props.numChannels)
					{
						for (unsigned int j = 0; j < m_Props.numChannels; j++)
						{
							targetBuffer[i + j] += (float)srcBuffer[i + j] * depthRatio;
						}
					}
				}

			}

			break;
		}

		case (DOUBLE_BIT_SIZE):
		{

		}

		default:
		{
			BB_CORE_ERROR("BIT DEPTH NOT ACCEPTED");
			return;
		}

		}
	}

	void Mixer::SetProc(std::shared_ptr<AudioProcessor> proc)
	{
		m_Procs.push_back(proc);
	}

	float Mixer::GetTypeRatio(unsigned short bitDepth1, unsigned short bitDepth2)
	{
		float max1 = 1.0f;
		float max2 = 1.0f;
		switch (bitDepth1)
		{

		case (BYTE_BIT_SIZE):
		{
			max1 = INT8_MAX;
			break;
		}

		case (INT16_BIT_SIZE):
		{
			max1 = INT16_MAX;
			break;
		}

		case (INT24_BIT_SIZE):
		{
			max1 = INT24_MAX;
			break;
		}

		case (FLOAT_BIT_SIZE):
		{
			max1 = 1.0f;
			break;
		}

		case (DOUBLE_BIT_SIZE):
		{
			max1 = 1.0f;
			break;
		}

		default:
		{
			return 0.0f;
		}

		}

		switch (bitDepth2)
		{

		case (BYTE_BIT_SIZE):
		{
			max2 = INT8_MAX;
			break;
		}

		case (INT16_BIT_SIZE):
		{
			max2 = INT16_MAX;
			break;
		}

		case (INT24_BIT_SIZE):
		{
			max2 = INT24_MAX;
			break;
		}

		case (FLOAT_BIT_SIZE):
		{
			max2 = 1.0f;
			break;
		}

		case (DOUBLE_BIT_SIZE):
		{
			max2 = 1.0f;
			break;
		}

		default:
		{
			return 0.0f;
		}

		}

		return max1 / max2;
	}
}