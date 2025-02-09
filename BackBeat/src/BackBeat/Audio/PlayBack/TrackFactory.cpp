#include "bbpch.h"

#include "BackBeat/Audio/FileIO/AudioFileReader.h"
#include "TrackFactory.h"
namespace BackBeat {

	static unsigned long s_DefaultFileMapSize = 1073741824ul; // 1 gb (placeholder, maybe place somewhere else)
	static FileMapper* s_FileMapper = nullptr;

	std::shared_ptr<Track> TrackFactory::BuildTrack(std::string filePath)
	{
		AudioInfo info = AudioFileReader::ReadFile(filePath);
		if (info.type == FileType::wav)
			return std::make_shared<Track>(info);

		return nullptr;
	}

	std::shared_ptr<Track> TrackFactory::BuildTempTrack(UUID id, AudioProps props)
	{
		auto filePath = FileSystem::GetTempDir().c_str() + id.ToString();
		AudioInfo info = {
			.type = FileType::recordingTemp,
			.name = id.ToString(),
			.filePath = filePath,
			.props = props,
			.dataZero = 0u,
			.dataSize = 0u
		};

		return std::make_shared<Track>(info);
	}

	std::shared_ptr<MappedTrack> TrackFactory::BuildMappedTrack(std::string filePath)
	{
		if (!s_FileMapper)
			return nullptr;

		AudioInfo info = AudioFileReader::ReadFile(filePath);
		if (info.type == FileType::wav)
			return std::make_shared<MappedTrack>(info, s_FileMapper->MapFile(filePath, info.props.fileSize));

		return nullptr;
	}

	std::shared_ptr<MappedTrack> TrackFactory::BuildMappedTrack(AudioInfo info)
	{
		if (!s_FileMapper)
			return nullptr;

		if (info.type == FileType::wav)
			return std::make_shared<MappedTrack>(info, s_FileMapper->MapFile(info.filePath, info.props.fileSize));

		return nullptr;
	}

	std::shared_ptr<MappedTrack> TrackFactory::BuildMappedTempTrack(UUID id, AudioProps props)
	{
		if (!s_FileMapper)
			return nullptr;

		auto filePath = FileSystem::GetTempDir().c_str() + id.ToString();
		AudioInfo info = {
			.type = FileType::recordingTemp,
			.name = id.ToString(),
			.filePath = filePath,
			.props = props,
			.dataZero = 0u,
			.dataSize = 0u
		};
		info.props.fileSize = s_DefaultFileMapSize;

		return std::make_shared<MappedTrack>(info, 
			s_FileMapper->CreateMappedFile(info.filePath, s_DefaultFileMapSize));
	}

	// Regular non-mapped tracks will need an empty file as standard c++ file I/O does not allow for overwrites.
	void TrackFactory::CopyTrackData(std::shared_ptr<Track> input, std::shared_ptr<Track> output)
	{
		if (!input || !output)
			return;

		AudioProps inProps = input->GetProps();
		AudioProps outProps = output->GetProps();
		unsigned int bufferSize = outProps.byteRate > inProps.byteRate ? outProps.byteRate : inProps.byteRate;
		output->SetName(input->GetName());

		if (inProps == outProps)
		{
			bool done = false;
			byte* buffer = new byte[bufferSize];

			input->SetPosition(0);
			output->SetPosition(0);

			// Tracks checks the bufferSize and checks if it is done 
			while (!done)
			{
				if (!input->Read(buffer, bufferSize))
					return;
				if (!output->Write(buffer, bufferSize))
					return;
				done = input->IsDone();
			}

			delete[bufferSize] buffer;
		}
		else if (inProps.sampleRate == outProps.sampleRate)
		{
			bool done = false;
			unsigned int numFrames = outProps.sampleRate;
			unsigned int inNumBytes = numFrames * inProps.blockAlign;
			unsigned int outNumBytes = numFrames * outProps.blockAlign;
			byte* inBuffer = new byte[bufferSize];
			byte* outBuffer = new byte[bufferSize];

			input->SetPosition(0);
			output->SetPosition(0);

			while (!done)
			{
				Audio::FlushBuffer(inBuffer, inNumBytes);
				Audio::FlushBuffer(outBuffer, outNumBytes);

				if (!input->Read(inBuffer, inNumBytes))
					return;

				switch (inProps.bitDepth)
				{

					case (Audio::ByteBitSize):
					{
						auto srcBuffer = reinterpret_cast<byte*>(inBuffer);
						if (outProps.bitDepth == Audio::ByteBitSize)
							Audio::TranslateDataToByte(srcBuffer, (byte*)outBuffer, inProps.bitDepth, outProps.numChannels, numFrames);
						else if (outProps.bitDepth == Audio::Int16BitSize)
							Audio::TranslateDataToShort(srcBuffer, (signed short*)outBuffer, inProps.bitDepth, outProps.numChannels, numFrames);
						else if (outProps.bitDepth == Audio::Int24BitSize)
							Audio::TranslateDataToInt24(srcBuffer, (byte*)outBuffer, inProps.bitDepth, outProps.numChannels, numFrames, inProps.bigEndian);
						else if (outProps.bitDepth == Audio::FloatBitSize)
							Audio::TranslateDataToFloat(srcBuffer, (float*)outBuffer, inProps.bitDepth, outProps.numChannels, numFrames);
						else if (outProps.bitDepth == Audio::DoubleBitSize)
							Audio::TranslateDataToDouble(srcBuffer, (double*)outBuffer, inProps.bitDepth, outProps.numChannels, numFrames);
						else
							BB_CORE_ERROR("Bit depth not supported");
						break;
					}

					case (Audio::Int16BitSize):
					{
						auto srcBuffer = reinterpret_cast<signed short*>(inBuffer);
						if (outProps.bitDepth == Audio::ByteBitSize)
							Audio::TranslateDataToByte(srcBuffer, (byte*)outBuffer, inProps.bitDepth, outProps.numChannels, numFrames);
						else if (outProps.bitDepth == Audio::Int16BitSize)
							Audio::TranslateDataToShort(srcBuffer, (signed short*)outBuffer, inProps.bitDepth, outProps.numChannels, numFrames);
						else if (outProps.bitDepth == Audio::Int24BitSize)
							Audio::TranslateDataToInt24(srcBuffer, (byte*)outBuffer, inProps.bitDepth, outProps.numChannels, numFrames, inProps.bigEndian);
						else if (outProps.bitDepth == Audio::FloatBitSize)
							Audio::TranslateDataToFloat(srcBuffer, (float*)outBuffer, inProps.bitDepth, outProps.numChannels, numFrames);
						else if (outProps.bitDepth == Audio::DoubleBitSize)
							Audio::TranslateDataToDouble(srcBuffer, (double*)outBuffer, inProps.bitDepth, outProps.numChannels, numFrames);
						else
							BB_CORE_ERROR("Bit depth not supported");
						break;
					}

					case (Audio::Int24BitSize):
					{
						auto buffer = reinterpret_cast<byte*>(inBuffer);
						int24* srcBuffer = int24::GetInt24Buffer(buffer, numFrames * outProps.numChannels, inProps.bigEndian);
						if (outProps.bitDepth == Audio::ByteBitSize)
							Audio::TranslateDataToByte(srcBuffer, (byte*)outBuffer, inProps.bitDepth, outProps.numChannels, numFrames);
						else if (outProps.bitDepth == Audio::Int16BitSize)
							Audio::TranslateDataToShort(srcBuffer, (signed short*)outBuffer, inProps.bitDepth, outProps.numChannels, numFrames);
						else if (outProps.bitDepth == Audio::Int24BitSize)
							Audio::TranslateDataToInt24(srcBuffer, (byte*)outBuffer, inProps.bitDepth, outProps.numChannels, numFrames, inProps.bigEndian);
						else if (outProps.bitDepth == Audio::FloatBitSize)
							Audio::TranslateDataToFloat(srcBuffer, (float*)outBuffer, inProps.bitDepth, outProps.numChannels, numFrames);
						else if (outProps.bitDepth == Audio::DoubleBitSize)
							Audio::TranslateDataToDouble(srcBuffer, (double*)outBuffer, inProps.bitDepth, outProps.numChannels, numFrames);
						else
							BB_CORE_ERROR("Bit depth not supported");
						delete[] srcBuffer;
						break;
					}

					case (Audio::FloatBitSize):
					{
						auto srcBuffer = reinterpret_cast<float*>(inBuffer);
						if (outProps.bitDepth == Audio::ByteBitSize)
							Audio::TranslateDataToByte(srcBuffer, (byte*)outBuffer, inProps.bitDepth, outProps.numChannels, numFrames);
						else if (outProps.bitDepth == Audio::Int16BitSize)
							Audio::TranslateDataToShort(srcBuffer, (signed short*)outBuffer, inProps.bitDepth, outProps.numChannels, numFrames);
						else if (outProps.bitDepth == Audio::Int24BitSize)
							Audio::TranslateDataToInt24(srcBuffer, (byte*)outBuffer, inProps.bitDepth, outProps.numChannels, numFrames, inProps.bigEndian);
						else if (outProps.bitDepth == Audio::FloatBitSize)
							Audio::TranslateDataToFloat(srcBuffer, (float*)outBuffer, inProps.bitDepth, outProps.numChannels, numFrames);
						else if (outProps.bitDepth == Audio::DoubleBitSize)
							Audio::TranslateDataToDouble(srcBuffer, (double*)outBuffer, inProps.bitDepth, outProps.numChannels, numFrames);
						else
							BB_CORE_ERROR("Bit depth not supported");
						break;
					}

					case (Audio::DoubleBitSize):
					{
						auto srcBuffer = reinterpret_cast<double*>(inBuffer);
						if (outProps.bitDepth == Audio::ByteBitSize)
							Audio::TranslateDataToByte(srcBuffer, (byte*)outBuffer, inProps.bitDepth, outProps.numChannels, numFrames);
						else if (outProps.bitDepth == Audio::Int16BitSize)
							Audio::TranslateDataToShort(srcBuffer, (signed short*)outBuffer, inProps.bitDepth, outProps.numChannels, numFrames);
						else if (outProps.bitDepth == Audio::Int24BitSize)
							Audio::TranslateDataToInt24(srcBuffer, (byte*)outBuffer, inProps.bitDepth, outProps.numChannels, numFrames, inProps.bigEndian);
						else if (outProps.bitDepth == Audio::FloatBitSize)
							Audio::TranslateDataToFloat(srcBuffer, (float*)outBuffer, inProps.bitDepth, outProps.numChannels, numFrames);
						else if (outProps.bitDepth == Audio::DoubleBitSize)
							Audio::TranslateDataToDouble(srcBuffer, (double*)outBuffer, inProps.bitDepth, outProps.numChannels, numFrames);
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

				if (!output->Write(outBuffer, outNumBytes))
					return;
				done = input->IsDone();
			}

			delete[bufferSize] inBuffer;
			delete[bufferSize] outBuffer;
		}
		else
			return;
	}

	void TrackFactory::SetFileMapper(FileMapper* fileMapper)
	{
		if (fileMapper)
			s_FileMapper = fileMapper;
	}

	void TrackFactory::SetFileSize(unsigned long size)
	{
		s_DefaultFileMapSize = size;
	}

}