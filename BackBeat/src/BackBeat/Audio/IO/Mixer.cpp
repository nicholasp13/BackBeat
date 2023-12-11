#include "bbpch.h"

#include "BackBeat/Audio/Helpers/int24.h"
#include "Mixer.h"
namespace BackBeat {

	Mixer::Mixer(AudioProps props)
		: m_NumProcs(0), m_Props(props), m_Procs(std::vector< std::shared_ptr<AudioProcessor> >())
	{

	}

	Mixer::~Mixer()
	{

	}

	// NOTE: 
	// - Upsampling, downsampling, and adjusting for number of channels will be handled by AudioProcessors
	// - This only handles adjusting for different bitdepths
	// - Only floating bit size case has been tested but the code for the others is there and should work
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
			byte* targetBuffer = reinterpret_cast<byte*>(data);
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
							targetBuffer[i + j] += (byte)((float)srcBuffer[i + j] * depthRatio);
						}
					}
				}
				else if (inProps.bitDepth == INT16_BIT_SIZE) {
					auto srcBuffer = reinterpret_cast<signed short*>(m_Procs[i]->GetOutputBuffer());

					for (unsigned int i = 0; i < numSamples * m_Props.numChannels; i += m_Props.numChannels)
					{
						for (unsigned int j = 0; j < m_Props.numChannels; j++)
						{
							targetBuffer[i + j] += (byte)((float)srcBuffer[i + j] * depthRatio);
						}
					}
				}

				else if (inProps.bitDepth == INT24_BIT_SIZE) {
					unsigned int pos = 0;
					auto srcBuffer = reinterpret_cast<byte*>(m_Procs[i]->GetOutputBuffer());
					byte srcBytes[3];
					int24 src = int24();

					for (unsigned int i = 0; i < numSamples * m_Props.numChannels; i += m_Props.numChannels)
					{
						for (unsigned int j = 0; j < m_Props.numChannels; j++)
						{
							for (unsigned int k = 0; k < 3; k++)
							{
								srcBytes[2 - k] = srcBuffer[pos];
								pos++;
							}
							src = int24(srcBytes[0], srcBytes[1], srcBytes[2]);
							targetBuffer[i + j] += (byte)(src.ToFloat() * depthRatio);
						}
					}

				}

				else if (inProps.bitDepth == FLOAT_BIT_SIZE) {
					auto srcBuffer = reinterpret_cast<float*>(m_Procs[i]->GetOutputBuffer());

					for (unsigned int i = 0; i < numSamples * m_Props.numChannels; i += m_Props.numChannels)
					{
						for (unsigned int j = 0; j < m_Props.numChannels; j++)
						{
							targetBuffer[i + j] += (byte)((float)srcBuffer[i + j] * depthRatio);
						}
					}
				}

				else if (inProps.bitDepth == DOUBLE_BIT_SIZE) {
					auto srcBuffer = reinterpret_cast<double*>(m_Procs[i]->GetOutputBuffer());

					for (unsigned int i = 0; i < numSamples * m_Props.numChannels; i += m_Props.numChannels)
					{
						for (unsigned int j = 0; j < m_Props.numChannels; j++)
						{
							targetBuffer[i + j] += (byte)((float)srcBuffer[i + j] * depthRatio);
						}
					}
				}

			}

			break;
		}

		case (INT16_BIT_SIZE):
		{
			short* targetBuffer = reinterpret_cast<short*>(data);
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
							targetBuffer[i + j] += (short)((float)srcBuffer[i + j] * depthRatio);
						}
					}
				}
				else if (inProps.bitDepth == INT16_BIT_SIZE) {
					auto srcBuffer = reinterpret_cast<signed short*>(m_Procs[i]->GetOutputBuffer());

					for (unsigned int i = 0; i < numSamples * m_Props.numChannels; i += m_Props.numChannels)
					{
						for (unsigned int j = 0; j < m_Props.numChannels; j++)
						{
							targetBuffer[i + j] += (short)((float)srcBuffer[i + j] * depthRatio);
						}
					}
				}

				else if (inProps.bitDepth == INT24_BIT_SIZE) {
					unsigned int pos = 0;
					auto srcBuffer = reinterpret_cast<byte*>(m_Procs[i]->GetOutputBuffer());
					byte srcBytes[3];
					int24 src = int24();

					for (unsigned int i = 0; i < numSamples * m_Props.numChannels; i += m_Props.numChannels)
					{
						for (unsigned int j = 0; j < m_Props.numChannels; j++)
						{
							for (unsigned int k = 0; k < 3; k++)
							{
								srcBytes[2 - k] = srcBuffer[pos];
								pos++;
							}
							src = int24(srcBytes[0], srcBytes[1], srcBytes[2]);
							targetBuffer[i + j] += (short)(src.ToFloat() * depthRatio);
						}
					}

				}

				else if (inProps.bitDepth == FLOAT_BIT_SIZE) {
					auto srcBuffer = reinterpret_cast<float*>(m_Procs[i]->GetOutputBuffer());

					for (unsigned int i = 0; i < numSamples * m_Props.numChannels; i += m_Props.numChannels)
					{
						for (unsigned int j = 0; j < m_Props.numChannels; j++)
						{
							targetBuffer[i + j] += (short)((float)srcBuffer[i + j] * depthRatio);
						}
					}
				}

				else if (inProps.bitDepth == DOUBLE_BIT_SIZE) {
					auto srcBuffer = reinterpret_cast<double*>(m_Procs[i]->GetOutputBuffer());

					for (unsigned int i = 0; i < numSamples * m_Props.numChannels; i += m_Props.numChannels)
					{
						for (unsigned int j = 0; j < m_Props.numChannels; j++)
						{
							targetBuffer[i + j] += (short)((float)srcBuffer[i + j] * depthRatio);
						}
					}
				}

			}

			break;
		}

		case (INT24_BIT_SIZE):
		{
			int24* targetBuffer = reinterpret_cast<int24*>(data);
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
							targetBuffer[i + j] += (int24)((float)srcBuffer[i + j] * depthRatio);
						}
					}
				}
				else if (inProps.bitDepth == INT16_BIT_SIZE) {
					auto srcBuffer = reinterpret_cast<signed short*>(m_Procs[i]->GetOutputBuffer());

					for (unsigned int i = 0; i < numSamples * m_Props.numChannels; i += m_Props.numChannels)
					{
						for (unsigned int j = 0; j < m_Props.numChannels; j++)
						{
							targetBuffer[i + j] += (int24)((float)srcBuffer[i + j] * depthRatio);
						}
					}
				}

				else if (inProps.bitDepth == INT24_BIT_SIZE) {
					unsigned int pos = 0;
					auto srcBuffer = reinterpret_cast<byte*>(m_Procs[i]->GetOutputBuffer());
					byte srcBytes[3];
					int24 src = int24();

					for (unsigned int i = 0; i < numSamples * m_Props.numChannels; i += m_Props.numChannels)
					{
						for (unsigned int j = 0; j < m_Props.numChannels; j++)
						{
							for (unsigned int k = 0; k < 3; k++)
							{
								srcBytes[2 - k] = srcBuffer[pos];
								pos++;
							}
							src = int24(srcBytes[0], srcBytes[1], srcBytes[2]);
							targetBuffer[i + j] += (int24)(src.ToFloat() * depthRatio);
						}
					}

				}

				else if (inProps.bitDepth == FLOAT_BIT_SIZE) {
					auto srcBuffer = reinterpret_cast<float*>(m_Procs[i]->GetOutputBuffer());

					for (unsigned int i = 0; i < numSamples * m_Props.numChannels; i += m_Props.numChannels)
					{
						for (unsigned int j = 0; j < m_Props.numChannels; j++)
						{
							targetBuffer[i + j] += (int24)((float)srcBuffer[i + j] * depthRatio);
						}
					}
				}

				else if (inProps.bitDepth == DOUBLE_BIT_SIZE) {
					auto srcBuffer = reinterpret_cast<double*>(m_Procs[i]->GetOutputBuffer());

					for (unsigned int i = 0; i < numSamples * m_Props.numChannels; i += m_Props.numChannels)
					{
						for (unsigned int j = 0; j < m_Props.numChannels; j++)
						{
							targetBuffer[i + j] += (int24)((float)srcBuffer[i + j] * depthRatio);
						}
					}
				}

			}
			break;
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
					unsigned int pos = 0;
					auto srcBuffer = reinterpret_cast<byte*>(m_Procs[i]->GetOutputBuffer());
					byte srcBytes[3];
					int24 src = int24();

					for (unsigned int i = 0; i < numSamples * m_Props.numChannels; i += m_Props.numChannels)
					{
						for (unsigned int j = 0; j < m_Props.numChannels; j++)
						{
							for (unsigned int k = 0; k < 3; k++) 
							{
								srcBytes[2 - k] = srcBuffer[pos];
								pos++;
							}
							src = int24(srcBytes[0], srcBytes[1], srcBytes[2]);
							targetBuffer[i + j] += src.ToFloat() * depthRatio;
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
							targetBuffer[i + j] += (double)srcBuffer[i + j] * depthRatio;
						}
					}
				}
				else if (inProps.bitDepth == INT16_BIT_SIZE) {
					auto srcBuffer = reinterpret_cast<signed short*>(m_Procs[i]->GetOutputBuffer());

					for (unsigned int i = 0; i < numSamples * m_Props.numChannels; i += m_Props.numChannels)
					{
						for (unsigned int j = 0; j < m_Props.numChannels; j++)
						{
							targetBuffer[i + j] += (double)srcBuffer[i + j] * depthRatio;
						}
					}
				}

				else if (inProps.bitDepth == INT24_BIT_SIZE) {
					unsigned int pos = 0;
					auto srcBuffer = reinterpret_cast<byte*>(m_Procs[i]->GetOutputBuffer());
					byte srcBytes[3];
					int24 src = int24();

					for (unsigned int i = 0; i < numSamples * m_Props.numChannels; i += m_Props.numChannels)
					{
						for (unsigned int j = 0; j < m_Props.numChannels; j++)
						{
							for (unsigned int k = 0; k < 3; k++)
							{
								srcBytes[2 - k] = srcBuffer[pos];
								pos++;
							}
							src = int24(srcBytes[0], srcBytes[1], srcBytes[2]);
							targetBuffer[i + j] += src.ToDouble() * depthRatio;
						}
					}

				}

				else if (inProps.bitDepth == FLOAT_BIT_SIZE) {
					auto srcBuffer = reinterpret_cast<float*>(m_Procs[i]->GetOutputBuffer());

					for (unsigned int i = 0; i < numSamples * m_Props.numChannels; i += m_Props.numChannels)
					{
						for (unsigned int j = 0; j < m_Props.numChannels; j++)
						{
							targetBuffer[i + j] += (double)srcBuffer[i + j] * depthRatio;
						}
					}
				}

				else if (inProps.bitDepth == DOUBLE_BIT_SIZE) {
					auto srcBuffer = reinterpret_cast<double*>(m_Procs[i]->GetOutputBuffer());

					for (unsigned int i = 0; i < numSamples * m_Props.numChannels; i += m_Props.numChannels)
					{
						for (unsigned int j = 0; j < m_Props.numChannels; j++)
						{
							targetBuffer[i + j] += (double)srcBuffer[i + j] * depthRatio;
						}
					}
				}

			}

			break;
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