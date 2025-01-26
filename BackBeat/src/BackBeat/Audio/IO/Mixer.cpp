#include "bbpch.h"

#include "Mixer.h"
namespace BackBeat {

	Mixer::Mixer()
		: 
		m_Buffer(new byte[s_BufferSize]),
		m_Props(AudioProps()), 
		m_RecordingSink(nullptr),
		m_VisualizingSink(nullptr),
		m_Processors(std::vector< std::shared_ptr<AudioProcessor> >()),
		m_RecordingManager(nullptr)
	{

	}

	Mixer::~Mixer()
	{

	}

	void Mixer::Init(AudioProps props, AudioSink* recordingSink, AudioSink* visualizingSink)
	{
		m_Props = props;
		m_RecordingSink = recordingSink;
		m_VisualizingSink = visualizingSink;
	}

	// NOTE: 
	// - Upsampling, downsampling, and adjusting for number of channels will be handled by AudioProcessors
	// - This only handles adjusting for different bitdepths
	// - Only floating bit size case has been tested but the code for the others is there and should work
	void Mixer::RenderData(byte* data, unsigned int numFrames)
	{
		if (numFrames == 0)
			return;

		for (unsigned int i = 0; i < m_Processors.size(); i++) {
			if (!m_Processors[i]->IsOn())
				continue;
			m_Processors[i]->ProcessSamples(numFrames, m_Props.sampleRate, m_Props.numChannels);
		}

		bool recording = false;
		if (m_RecordingManager)
			recording = m_RecordingManager->IsRecording();
		unsigned int totalBytes = numFrames * m_Props.blockAlign;
		unsigned int totalSamples = numFrames * m_Props.numChannels;
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
					Audio::TranslateDataToByte(srcBuffer, (byte*)data, inProps.bitDepth, m_Props.numChannels, numFrames);
				else if (m_Props.bitDepth == Audio::Int16BitSize)
					Audio::TranslateDataToShort(srcBuffer, (signed short*)data, inProps.bitDepth, m_Props.numChannels, numFrames);
				else if (m_Props.bitDepth == Audio::Int24BitSize)
					Audio::TranslateDataToInt24(srcBuffer, (byte*)data, inProps.bitDepth, m_Props.numChannels, numFrames, inProps.bigEndian);
				else if (m_Props.bitDepth == Audio::FloatBitSize)
					Audio::TranslateDataToFloat(srcBuffer, (float*)data, inProps.bitDepth, m_Props.numChannels, numFrames);
				else if (m_Props.bitDepth == Audio::DoubleBitSize)
					Audio::TranslateDataToDouble(srcBuffer, (double*)data, inProps.bitDepth, m_Props.numChannels, numFrames);
				else
					BB_CORE_ERROR("Bit depth not supported");
				break;
			}

			case (Audio::Int16BitSize):
			{
				auto srcBuffer = reinterpret_cast<signed short*>(inBuffer);
				if (m_Props.bitDepth == Audio::ByteBitSize)
					Audio::TranslateDataToByte(srcBuffer, (byte*)data, inProps.bitDepth, m_Props.numChannels, numFrames);
				else if (m_Props.bitDepth == Audio::Int16BitSize)
					Audio::TranslateDataToShort(srcBuffer, (signed short*)data, inProps.bitDepth, m_Props.numChannels, numFrames);
				else if (m_Props.bitDepth == Audio::Int24BitSize)
					Audio::TranslateDataToInt24(srcBuffer, (byte*)data, inProps.bitDepth, m_Props.numChannels, numFrames, inProps.bigEndian);
				else if (m_Props.bitDepth == Audio::FloatBitSize)
					Audio::TranslateDataToFloat(srcBuffer, (float*)data, inProps.bitDepth, m_Props.numChannels, numFrames);
				else if (m_Props.bitDepth == Audio::DoubleBitSize)
					Audio::TranslateDataToDouble(srcBuffer, (double*)data, inProps.bitDepth, m_Props.numChannels, numFrames);
				else
					BB_CORE_ERROR("Bit depth not supported");
				break;
			}

			// NOTE: Allocating memory on the heap during important run time ESPECIALLY on the audio thread which requires
			//       exact timing is generally looked down on. However in this case, this code is baked in here and in 
			//       SampleBuilder and would take a bit of reworking to fix so this is for future reference in case I forget.
			//       (Another thing is that creating an array or pointer of the class as a private member caused nontrivial compiler 
			//       errors that made this tough to solve)
			case (Audio::Int24BitSize):
			{
				auto buffer = reinterpret_cast<byte*>(inBuffer);
				int24* srcBuffer = int24::GetInt24Buffer(buffer, numFrames * m_Props.numChannels, inProps.bigEndian);
				if (m_Props.bitDepth == Audio::ByteBitSize)
					Audio::TranslateDataToByte(srcBuffer, (byte*)data, inProps.bitDepth, m_Props.numChannels, numFrames);
				else if (m_Props.bitDepth == Audio::Int16BitSize)
					Audio::TranslateDataToShort(srcBuffer, (signed short*)data, inProps.bitDepth, m_Props.numChannels, numFrames);
				else if (m_Props.bitDepth == Audio::Int24BitSize)
					Audio::TranslateDataToInt24(srcBuffer, (byte*)data, inProps.bitDepth, m_Props.numChannels, numFrames, inProps.bigEndian);
				else if (m_Props.bitDepth == Audio::FloatBitSize)
					Audio::TranslateDataToFloat(srcBuffer, (float*)data, inProps.bitDepth, m_Props.numChannels, numFrames);
				else if (m_Props.bitDepth == Audio::DoubleBitSize)
					Audio::TranslateDataToDouble(srcBuffer, (double*)data, inProps.bitDepth, m_Props.numChannels, numFrames);
				else
					BB_CORE_ERROR("Bit depth not supported");
				delete[] srcBuffer;
				break;
			}

			case (Audio::FloatBitSize):
			{
				auto srcBuffer = reinterpret_cast<float*>(inBuffer);
				if (m_Props.bitDepth == Audio::ByteBitSize)
					Audio::TranslateDataToByte(srcBuffer, (byte*)data, inProps.bitDepth, m_Props.numChannels, numFrames);
				else if (m_Props.bitDepth == Audio::Int16BitSize)
					Audio::TranslateDataToShort(srcBuffer, (signed short*)data, inProps.bitDepth, m_Props.numChannels, numFrames);
				else if (m_Props.bitDepth == Audio::Int24BitSize)
					Audio::TranslateDataToInt24(srcBuffer, (byte*)data, inProps.bitDepth, m_Props.numChannels, numFrames, inProps.bigEndian);
				else if (m_Props.bitDepth == Audio::FloatBitSize)
					Audio::TranslateDataToFloat(srcBuffer, (float*)data, inProps.bitDepth, m_Props.numChannels, numFrames);
				else if (m_Props.bitDepth == Audio::DoubleBitSize)
					Audio::TranslateDataToDouble(srcBuffer, (double*)data, inProps.bitDepth, m_Props.numChannels, numFrames);
				else
					BB_CORE_ERROR("Bit depth not supported");
				break;
			}

			case (Audio::DoubleBitSize):
			{
				auto srcBuffer = reinterpret_cast<double*>(inBuffer);
				if (m_Props.bitDepth == Audio::ByteBitSize)
					Audio::TranslateDataToByte(srcBuffer, (byte*)data, inProps.bitDepth, m_Props.numChannels, numFrames);
				else if (m_Props.bitDepth == Audio::Int16BitSize)
					Audio::TranslateDataToShort(srcBuffer, (signed short*)data, inProps.bitDepth, m_Props.numChannels, numFrames);
				else if (m_Props.bitDepth == Audio::Int24BitSize)
					Audio::TranslateDataToInt24(srcBuffer, (byte*)data, inProps.bitDepth, m_Props.numChannels, numFrames, inProps.bigEndian);
				else if (m_Props.bitDepth == Audio::FloatBitSize)
					Audio::TranslateDataToFloat(srcBuffer, (float*)data, inProps.bitDepth, m_Props.numChannels, numFrames);
				else if (m_Props.bitDepth == Audio::DoubleBitSize)
					Audio::TranslateDataToDouble(srcBuffer, (double*)data, inProps.bitDepth, m_Props.numChannels, numFrames);
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

			if (recording)
			{
				auto id = m_Processors[i]->GetID();
				if (m_RecordingSink && m_RecordingManager->IsActive(id))
					m_RecordingSink->WriteData(reinterpret_cast<float*>(inBuffer), totalSamples);
			}

		}

		if (m_Visualizer->IsOn())
			m_VisualizingSink->WriteData(reinterpret_cast<float*>(data), totalSamples);
	}

	void Mixer::RenderData(unsigned int numFrames)
	{
		if (numFrames == 0)
			return;

		byte* data = m_Buffer.get();

		for (unsigned int i = 0; i < m_Processors.size(); i++) {
			if (!m_Processors[i]->IsOn())
				continue;
			m_Processors[i]->ProcessSamples(numFrames, m_Props.sampleRate, m_Props.numChannels);
		}

		bool recording = false;
		if (m_RecordingManager)
			recording = m_RecordingManager->IsRecording();

		unsigned int totalBytes = numFrames * m_Props.blockAlign;
		unsigned int totalSamples = numFrames * m_Props.numChannels;

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
					Audio::TranslateDataToByte(srcBuffer, (byte*)data, inProps.bitDepth, m_Props.numChannels, numFrames);
				else if (m_Props.bitDepth == Audio::Int16BitSize)
					Audio::TranslateDataToShort(srcBuffer, (signed short*)data, inProps.bitDepth, m_Props.numChannels, numFrames);
				else if (m_Props.bitDepth == Audio::Int24BitSize)
					Audio::TranslateDataToInt24(srcBuffer, (byte*)data, inProps.bitDepth, m_Props.numChannels, numFrames, inProps.bigEndian);
				else if (m_Props.bitDepth == Audio::FloatBitSize)
					Audio::TranslateDataToFloat(srcBuffer, (float*)data, inProps.bitDepth, m_Props.numChannels, numFrames);
				else if (m_Props.bitDepth == Audio::DoubleBitSize)
					Audio::TranslateDataToDouble(srcBuffer, (double*)data, inProps.bitDepth, m_Props.numChannels, numFrames);
				else
					BB_CORE_ERROR("Bit depth not supported");
				break;
			}

			case (Audio::Int16BitSize):
			{
				auto srcBuffer = reinterpret_cast<signed short*>(inBuffer);
				if (m_Props.bitDepth == Audio::ByteBitSize)
					Audio::TranslateDataToByte(srcBuffer, (byte*)data, inProps.bitDepth, m_Props.numChannels, numFrames);
				else if (m_Props.bitDepth == Audio::Int16BitSize)
					Audio::TranslateDataToShort(srcBuffer, (signed short*)data, inProps.bitDepth, m_Props.numChannels, numFrames);
				else if (m_Props.bitDepth == Audio::Int24BitSize)
					Audio::TranslateDataToInt24(srcBuffer, (byte*)data, inProps.bitDepth, m_Props.numChannels, numFrames, inProps.bigEndian);
				else if (m_Props.bitDepth == Audio::FloatBitSize)
					Audio::TranslateDataToFloat(srcBuffer, (float*)data, inProps.bitDepth, m_Props.numChannels, numFrames);
				else if (m_Props.bitDepth == Audio::DoubleBitSize)
					Audio::TranslateDataToDouble(srcBuffer, (double*)data, inProps.bitDepth, m_Props.numChannels, numFrames);
				else
					BB_CORE_ERROR("Bit depth not supported");
				break;
			}

			// NOTE: Allocating memory on the heap during important run time ESPECIALLY on the audio thread which requires
			//       exact timing is generally looked down on. However in this case, this code is baked in here and in 
			//       SampleBuilder and would take a bit of reworking to fix so this is for future reference in case I forget.
			//       (Another thing is that creating an array or pointer of the class as a private member caused nontrivial compiler 
			//       errors that made this tough to solve)
			case (Audio::Int24BitSize):
			{
				auto buffer = reinterpret_cast<byte*>(inBuffer);
				int24* srcBuffer = int24::GetInt24Buffer(buffer, numFrames * m_Props.numChannels, inProps.bigEndian);
				if (m_Props.bitDepth == Audio::ByteBitSize)
					Audio::TranslateDataToByte(srcBuffer, (byte*)data, inProps.bitDepth, m_Props.numChannels, numFrames);
				else if (m_Props.bitDepth == Audio::Int16BitSize)
					Audio::TranslateDataToShort(srcBuffer, (signed short*)data, inProps.bitDepth, m_Props.numChannels, numFrames);
				else if (m_Props.bitDepth == Audio::Int24BitSize)
					Audio::TranslateDataToInt24(srcBuffer, (byte*)data, inProps.bitDepth, m_Props.numChannels, numFrames, inProps.bigEndian);
				else if (m_Props.bitDepth == Audio::FloatBitSize)
					Audio::TranslateDataToFloat(srcBuffer, (float*)data, inProps.bitDepth, m_Props.numChannels, numFrames);
				else if (m_Props.bitDepth == Audio::DoubleBitSize)
					Audio::TranslateDataToDouble(srcBuffer, (double*)data, inProps.bitDepth, m_Props.numChannels, numFrames);
				else
					BB_CORE_ERROR("Bit depth not supported");
				delete[] srcBuffer;
				break;
			}

			case (Audio::FloatBitSize):
			{
				auto srcBuffer = reinterpret_cast<float*>(inBuffer);
				if (m_Props.bitDepth == Audio::ByteBitSize)
					Audio::TranslateDataToByte(srcBuffer, (byte*)data, inProps.bitDepth, m_Props.numChannels, numFrames);
				else if (m_Props.bitDepth == Audio::Int16BitSize)
					Audio::TranslateDataToShort(srcBuffer, (signed short*)data, inProps.bitDepth, m_Props.numChannels, numFrames);
				else if (m_Props.bitDepth == Audio::Int24BitSize)
					Audio::TranslateDataToInt24(srcBuffer, (byte*)data, inProps.bitDepth, m_Props.numChannels, numFrames, inProps.bigEndian);
				else if (m_Props.bitDepth == Audio::FloatBitSize)
					Audio::TranslateDataToFloat(srcBuffer, (float*)data, inProps.bitDepth, m_Props.numChannels, numFrames);
				else if (m_Props.bitDepth == Audio::DoubleBitSize)
					Audio::TranslateDataToDouble(srcBuffer, (double*)data, inProps.bitDepth, m_Props.numChannels, numFrames);
				else
					BB_CORE_ERROR("Bit depth not supported");
				break;
			}

			case (Audio::DoubleBitSize):
			{
				auto srcBuffer = reinterpret_cast<double*>(inBuffer);
				if (m_Props.bitDepth == Audio::ByteBitSize)
					Audio::TranslateDataToByte(srcBuffer, (byte*)data, inProps.bitDepth, m_Props.numChannels, numFrames);
				else if (m_Props.bitDepth == Audio::Int16BitSize)
					Audio::TranslateDataToShort(srcBuffer, (signed short*)data, inProps.bitDepth, m_Props.numChannels, numFrames);
				else if (m_Props.bitDepth == Audio::Int24BitSize)
					Audio::TranslateDataToInt24(srcBuffer, (byte*)data, inProps.bitDepth, m_Props.numChannels, numFrames, inProps.bigEndian);
				else if (m_Props.bitDepth == Audio::FloatBitSize)
					Audio::TranslateDataToFloat(srcBuffer, (float*)data, inProps.bitDepth, m_Props.numChannels, numFrames);
				else if (m_Props.bitDepth == Audio::DoubleBitSize)
					Audio::TranslateDataToDouble(srcBuffer, (double*)data, inProps.bitDepth, m_Props.numChannels, numFrames);
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

			if (recording)
			{
				auto id = m_Processors[i]->GetID();
				if (m_RecordingSink && m_RecordingManager->IsActive(id))
					m_RecordingSink->WriteData(reinterpret_cast<float*>(inBuffer), totalSamples);
			}

		}

		if (m_Visualizer->IsOn())
			m_VisualizingSink->WriteData(reinterpret_cast<float*>(data), totalSamples);
	}

	void Mixer::DeleteProcessor(UUID id)
	{
		for (auto it = m_Processors.begin(); it != m_Processors.end(); it++)
		{
			if ((*it)->GetID() == id)
			{
				m_Processors.erase(it);
				return;
			}
		}
	}
}