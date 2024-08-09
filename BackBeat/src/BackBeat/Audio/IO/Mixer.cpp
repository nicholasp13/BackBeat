#include "bbpch.h"

#include "BackBeat/Audio/Helpers/int24.h"
#include "Mixer.h"
namespace BackBeat {

	Mixer::Mixer(AudioProps props)
		: m_NumProcessors(0), m_Props(props), m_Processors(std::vector< std::shared_ptr<AudioProcessor> >())
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
		for (unsigned int i = 0; i < m_Processors.size(); i++) {
			if (!m_Processors[i]->IsOn())
				continue;
			m_Processors[i]->ProcessSamples(numSamples, m_Props.sampleRate, m_Props.numChannels); // TODO: Multithread this call
		}

		bool recording = false;
		if (m_RecordingManager)
			recording = m_RecordingManager->IsRecording();

		unsigned int totalBytes = numSamples * m_Props.blockAlign;
		byte defaultVal = 0x00;
		Audio::FlushBufferT(data, &defaultVal, totalBytes);

		for (unsigned int i = 0; i < m_Processors.size(); i++) {
			if (!m_Processors[i]->IsOn())
				continue;

			auto inBuffer = m_Processors[i]->GetOutputBuffer();
			AudioProps inProps = m_Processors[i]->GetProperties();
			switch (inProps.bitDepth)
			{

			case (Audio::ByteBitSize):
			{
				auto srcBuffer = reinterpret_cast<byte*>(inBuffer);
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
				auto srcBuffer = reinterpret_cast<signed short*>(inBuffer);
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
				auto buffer = reinterpret_cast<byte*>(inBuffer);
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
				auto srcBuffer = reinterpret_cast<float*>(inBuffer);
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
				auto srcBuffer = reinterpret_cast<double*>(inBuffer);
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

			// FIXME: This call to record lags Synth enough to ruin the audio. 
			//        Need to implement some sort of multithreading in regards to Creating, Rendering, Recording audio data
			if (recording)
			{
				auto id = m_Processors[i]->GetID();
				m_RecordingManager->Record(id, inBuffer, numSamples * inProps.numChannels);
			}
		}

	}

	void Mixer::PushProcessor(std::shared_ptr<AudioProcessor> processor)
	{
		m_Processors.push_back(processor);
	}
}