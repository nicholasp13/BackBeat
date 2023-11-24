#pragma once

// TODO: Fully implement by changing writing to and connecting to ModSources and ModDestinations

#include "BackBeat/Audio/Synth/SynthBase.h"
#include "BackBeat/Audio/Synth/SynthParameters.h"
namespace BackBeat {

	class ModularMatrix 
	{
	public:
		ModularMatrix(std::shared_ptr<ModMatrixParameters> params);
		~ModularMatrix();

		void AddModSource(std::shared_ptr<float[]> source, ModSources index);
		void AddModDestination(std::shared_ptr<float[]> destination, ModDestinations index);
		void ClearModSource(ModSources index);
		void ClearModDestination(ModDestinations index);
		void RunModMatrix(UINT32 numSamples);

		std::shared_ptr<ModMatrixParameters> GetParams() { return m_Params; }

	private:
		std::shared_ptr<ModMatrixParameters> m_Params;
		std::shared_ptr<float[]> m_ModSources[NumModSources];
		std::shared_ptr<float[]> m_ModDestinations[NumModDestinations];
	};
}