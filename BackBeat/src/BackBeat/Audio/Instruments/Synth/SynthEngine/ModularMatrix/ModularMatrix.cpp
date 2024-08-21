#include "bbpch.h"

#include "ModularMatrix.h"
namespace BackBeat {

	ModularMatrix::ModularMatrix(std::shared_ptr<ModMatrixParameters> params)
		:
		m_Params(params)
	{

	}

	ModularMatrix::~ModularMatrix()
	{

	}

	void ModularMatrix::AddModSource(std::shared_ptr<float[]> source, ModSources index)
	{
		m_ModSources[index] = source;
	}

	void ModularMatrix::AddModDestination(std::shared_ptr<float[]> destination, ModDestinations index)
	{
		m_ModDestinations[index] = destination;
	}

	// TODO: Implement if needed for user programming 
	void ModularMatrix::ClearModSource(ModSources index)
	{

	}
	
	// TODO: Implement if needed for user programming 
	void ModularMatrix::ClearModDestination(ModDestinations index)
	{
	
	}
	
	void ModularMatrix::RunModMatrix(unsigned int numSamples)
	{
		unsigned int totalSamples = numSamples * Audio::Stereo;
		float sourceIntensity = 0.0f;
		float destinationIntensity = 0.0f;
		float channelIntensity = 0.0f;
		ModularDestination destination;
		std::shared_ptr<float[]> srcOutputBuffer;
		std::shared_ptr<float[]> destinationInputBuffer;

		for (unsigned int i = 0; i < NumModDestinations; i++)
		{
			destination = m_Params->modDestinationsCols[i];
			destinationIntensity = destination.intensity;
			destinationInputBuffer = m_ModDestinations[i];

			for (unsigned int j = 0; j < NumModSources; j++)
			{
				if (!destination.channelOn[j])
					break;
				sourceIntensity = m_Params->modSourcesRows[j].intensity;
				channelIntensity = m_Params->channelIntensity[j];
				srcOutputBuffer = m_ModSources[j];
				for (unsigned int k = 0; k < totalSamples; k++)
				{
					// NOTE: Amp modulators are multiplicative and input modulators are additive. Implementation subject to change
					destinationInputBuffer[k] += srcOutputBuffer[k] 
						* destinationIntensity * sourceIntensity * channelIntensity;
				}
			}
		}
	}
}