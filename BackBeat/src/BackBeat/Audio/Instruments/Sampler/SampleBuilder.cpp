#include "bbpch.h"

#include "SampleBuilder.h"
namespace BackBeat {

	void SampleBuilder::SaveSample(std::shared_ptr<Track> track, unsigned int start, unsigned int end)
	{
		if (!track)
			return;
		if (end <= start)
			return;

		AudioInfo trackInfo = track->GetInfo();
		if (trackInfo.type != FileType::wav)
			return;

		AudioProps trackProps = trackInfo.props;
		const unsigned int sampleRate = 48000;
		// TODO: Change when an upsampler and downsampler are created
		if (trackProps.sampleRate != sampleRate)
			return;

		std::string filePath = FileDialog::SaveFile("Sample Files (*.smpl)\0*.smpl\0");
		if (strcmp(filePath.c_str(), "") == 0)
			return;

		// Arbitrary restriction on SAMPLE size
		const unsigned int maxSeconds = 10;
		const unsigned int maxTime = sampleRate * Audio::FloatByteSize * maxSeconds;
		if (end - start > maxTime)
			return;

		const unsigned int format = 3;
		const unsigned int byteRate = 384000;
		const unsigned int blockAlign = 8;
		const float blockAlignRatio = (float)blockAlign / (float)trackProps.blockAlign;
		const unsigned int trackSize = (end - (end % trackProps.blockAlign))
			- (start - (start % trackProps.blockAlign));
		const unsigned int dataSize = (unsigned int)((float)(trackSize) * blockAlignRatio);

		AudioProps sampleProps = AudioProps();
		sampleProps.bigEndian = false;
		sampleProps.numChannels = Audio::Stereo;
		sampleProps.format = format;
		sampleProps.sampleRate = sampleRate;
		sampleProps.byteRate = byteRate;
		sampleProps.blockAlign = blockAlign;
		sampleProps.bitDepth = Audio::FloatBitSize;
		sampleProps.fileSize = Audio::SAMPLETotalHeaderSize + dataSize;

		bool success = AudioFileWriter::WriteSampleFileHeader(filePath, sampleProps, dataSize);
		if (success)
		{
			const unsigned int trackIncrementSize = trackProps.byteRate;
			const unsigned int trackFileSize = trackSize + Audio::WAVTotalHeaderSize;
			const unsigned int sampleDataIncrementSize = sampleProps.byteRate;
			unsigned int filePosition = Audio::WAVTotalHeaderSize;
			unsigned int trackIncrement = trackIncrementSize;
			unsigned int sampleDataIncrement = sampleDataIncrementSize;
			unsigned int numSamples = sampleDataIncrement / sampleProps.blockAlign;
			char* trackData = new char[trackIncrementSize];
			char* sampleData = new char[sampleDataIncrementSize];
			
			track->SetStart(start);
			track->SetEnd(end);
			track->SetPosition(start);
			track->SetVolume(1.0f);
			while (filePosition < trackFileSize && success)
			{
				trackIncrement = (trackIncrementSize + filePosition) <= trackFileSize 
					? trackIncrementSize : (trackFileSize - filePosition);
				sampleDataIncrement = (unsigned int)((float)trackIncrement * blockAlignRatio);
				numSamples = sampleDataIncrement / sampleProps.blockAlign;

				track->Render((byte*)trackData, trackIncrement);

				byte defaultValue = 0x00;
				Audio::FlushBufferT((byte*)sampleData, &defaultValue, sampleDataIncrement);

				switch (trackProps.bitDepth)
				{

				case(Audio::ByteBitSize):
				{
					Audio::TranslateDataToFloat((byte*)trackData, (float*)sampleData, 
						trackProps.bitDepth, Audio::Stereo, numSamples);
					break;
				}

				case(Audio::Int16BitSize):
				{
					Audio::TranslateDataToFloat((signed short*)trackData, (float*)sampleData,
						trackProps.bitDepth, Audio::Stereo, numSamples);
					break;
				}

				case(Audio::Int24BitSize):
				{
					int24* intBuffer = int24::GetInt24Buffer((byte*)trackData, numSamples * sampleProps.numChannels, 
						sampleProps.bigEndian);
					Audio::TranslateDataToFloat(intBuffer, (float*)sampleData,
						trackProps.bitDepth, Audio::Stereo, numSamples);
					delete[] intBuffer;
					break;
				}

				case(Audio::FloatBitSize):
				{
					Audio::TranslateDataToFloat((float*)trackData, (float*)sampleData,
						trackProps.bitDepth, Audio::Stereo, numSamples);
					break;
				}

				case(Audio::DoubleBitSize):
				{
					Audio::TranslateDataToFloat((double*)trackData, (float*)sampleData,
						trackProps.bitDepth, Audio::Stereo, numSamples);
					break;
				}
				default:
				{
					delete[] trackData;
					delete[] sampleData;
					BB_CORE_ERROR("Bit depth not supported");
					return;
				}

				}

				success = AudioFileWriter::WriteAudioFileData(filePath, sampleData, sampleDataIncrement);
				filePosition += trackIncrement;
			}
			delete[] trackData;
			delete[] sampleData;
		}
	}

	Sample* SampleBuilder::BuildSample(std::string filePath)
	{
		AudioInfo info = AudioFileReader::ReadFile(filePath);
		if (info.type == FileType::sample)
			return new Sample(info);

		return nullptr;
	}

}