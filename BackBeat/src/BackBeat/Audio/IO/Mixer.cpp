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
		for (unsigned int k = 0; k < numSamples * m_Props.blockAlign; k++) {
			data[k] = (byte)0x00;
		}

		for (unsigned int i = 0; i < m_Procs.size(); i++) {
			if (!m_Procs[i]->IsOn())
				continue;

			AudioProps inProps = m_Procs[i]->GetProperties();
			switch (inProps.bitDepth)
			{

			case (Audio::ByteBitSize):
			{
				auto srcBuffer = reinterpret_cast<byte*>(m_Procs[i]->GetOutputBuffer());
				if (m_Props.bitDepth == Audio::ByteBitSize)
					Audio::TranslateDataToByte(srcBuffer, (byte*)data, inProps.bitDepth, m_Props.numChannels, numSamples);
				else if (m_Props.bitDepth == Audio::Int16BitSize)
					Audio::TranslateDataToShort(srcBuffer, (signed short*)data, inProps.bitDepth, m_Props.numChannels, numSamples);
				else if (m_Props.bitDepth == Audio::Int24BitSize)
					int24::TranslateDataToInt24(srcBuffer, (byte*)data, inProps.bitDepth, m_Props.numChannels, numSamples, inProps.bigEndian);
				else if (m_Props.bitDepth == Audio::FloatBitSize)
					Audio::TranslateDataToFloat(srcBuffer, (float*)data, inProps.bitDepth, m_Props.numChannels, numSamples);
				else if (m_Props.bitDepth == Audio::DoubleBitSize)
					Audio::TranslateDataToDouble(srcBuffer, (double*)data, inProps.bitDepth, m_Props.numChannels, numSamples);
				else
					BB_CORE_ERROR("Bit depth not supported");
				break;
			}

			case (Audio::Int16BitSize):
			{
				auto srcBuffer = reinterpret_cast<signed short*>(m_Procs[i]->GetOutputBuffer());
				if (m_Props.bitDepth == Audio::ByteBitSize)
					Audio::TranslateDataToByte(srcBuffer, (byte*)data, inProps.bitDepth, m_Props.numChannels, numSamples);
				else if (m_Props.bitDepth == Audio::Int16BitSize)
					Audio::TranslateDataToShort(srcBuffer, (signed short*)data, inProps.bitDepth, m_Props.numChannels, numSamples);
				else if (m_Props.bitDepth == Audio::Int24BitSize)
					int24::TranslateDataToInt24(srcBuffer, (byte*)data, inProps.bitDepth, m_Props.numChannels, numSamples, inProps.bigEndian);
				else if (m_Props.bitDepth == Audio::FloatBitSize)
					Audio::TranslateDataToFloat(srcBuffer, (float*)data, inProps.bitDepth, m_Props.numChannels, numSamples);
				else if (m_Props.bitDepth == Audio::DoubleBitSize)
					Audio::TranslateDataToDouble(srcBuffer, (double*)data, inProps.bitDepth, m_Props.numChannels, numSamples);
				else
					BB_CORE_ERROR("Bit depth not supported");
				break;
			}

			case (Audio::Int24BitSize):
			{
				auto buffer = reinterpret_cast<byte*>(m_Procs[i]->GetOutputBuffer());
				int24* srcBuffer = int24::GetInt24Buffer(buffer, numSamples * m_Props.numChannels, inProps.bigEndian);
				if (m_Props.bitDepth == Audio::ByteBitSize)
					Audio::TranslateDataToByte(srcBuffer, (byte*)data, inProps.bitDepth, m_Props.numChannels, numSamples);
				else if (m_Props.bitDepth == Audio::Int16BitSize)
					Audio::TranslateDataToShort(srcBuffer, (signed short*)data, inProps.bitDepth, m_Props.numChannels, numSamples);
				else if (m_Props.bitDepth == Audio::Int24BitSize)
					int24::TranslateDataToInt24(srcBuffer, (byte*)data, inProps.bitDepth, m_Props.numChannels, numSamples, inProps.bigEndian);
				else if (m_Props.bitDepth == Audio::FloatBitSize)
					Audio::TranslateDataToFloat(srcBuffer, (float*)data, inProps.bitDepth, m_Props.numChannels, numSamples);
				else if (m_Props.bitDepth == Audio::DoubleBitSize)
					Audio::TranslateDataToDouble(srcBuffer, (double*)data, inProps.bitDepth, m_Props.numChannels, numSamples);
				else
					BB_CORE_ERROR("Bit depth not supported");
				delete[] srcBuffer;
				break;
			}

			case (Audio::FloatBitSize):
			{
				auto srcBuffer = reinterpret_cast<float*>(m_Procs[i]->GetOutputBuffer());
				if (m_Props.bitDepth == Audio::ByteBitSize)
					Audio::TranslateDataToByte(srcBuffer, (byte*)data, inProps.bitDepth, m_Props.numChannels, numSamples);
				else if (m_Props.bitDepth == Audio::Int16BitSize)
					Audio::TranslateDataToShort(srcBuffer, (signed short*)data, inProps.bitDepth, m_Props.numChannels, numSamples);
				else if (m_Props.bitDepth == Audio::Int24BitSize)
					int24::TranslateDataToInt24(srcBuffer, (byte*)data, inProps.bitDepth, m_Props.numChannels, numSamples, inProps.bigEndian);
				else if (m_Props.bitDepth == Audio::FloatBitSize)
					Audio::TranslateDataToFloat(srcBuffer, (float*)data, inProps.bitDepth, m_Props.numChannels, numSamples);
				else if (m_Props.bitDepth == Audio::DoubleBitSize)
					Audio::TranslateDataToDouble(srcBuffer, (double*)data, inProps.bitDepth, m_Props.numChannels, numSamples);
				else
					BB_CORE_ERROR("Bit depth not supported");
				break;
			}

			case (Audio::DoubleBitSize):
			{
				auto srcBuffer = reinterpret_cast<double*>(m_Procs[i]->GetOutputBuffer());
				if (m_Props.bitDepth == Audio::ByteBitSize)
					Audio::TranslateDataToByte(srcBuffer, (byte*)data, inProps.bitDepth, m_Props.numChannels, numSamples);
				else if (m_Props.bitDepth == Audio::Int16BitSize)
					Audio::TranslateDataToShort(srcBuffer, (signed short*)data, inProps.bitDepth, m_Props.numChannels, numSamples);
				else if (m_Props.bitDepth == Audio::Int24BitSize)
					int24::TranslateDataToInt24(srcBuffer, (byte*)data, inProps.bitDepth, m_Props.numChannels, numSamples, inProps.bigEndian);
				else if (m_Props.bitDepth == Audio::FloatBitSize)
					Audio::TranslateDataToFloat(srcBuffer, (float*)data, inProps.bitDepth, m_Props.numChannels, numSamples);
				else if (m_Props.bitDepth == Audio::DoubleBitSize)
					Audio::TranslateDataToDouble(srcBuffer, (double*)data, inProps.bitDepth, m_Props.numChannels, numSamples);
				else
					BB_CORE_ERROR("Bit depth not supported");
				break;
			}

			default:
			{
				BB_CORE_ERROR("Bit depth not supported");
				break;
			}

			}

		}

	}

	void Mixer::SetProc(std::shared_ptr<AudioProcessor> proc)
	{
		m_Procs.push_back(proc);
	}
}