#include "bbpch.h"

#include "WAVFileBuilder.h"
#include "BackBeat/File/FileDialog.h"
namespace BackBeat {

	bool WAVFileBuilder::BuildWAVFile(Track* track, unsigned int start, unsigned int end)
	{
		if (!track)
			return false;
		if (end <= start)
			return false;

		std::string filePath = FileDialog::SaveFile("WAV Files (*.wav)\0*.wav\0");
		AudioProps props = track->GetProps();
		unsigned int arraySize = props.byteRate;
		char* data = new char[arraySize];
		unsigned int size = (end - (end % props.blockAlign)) - (start - (start % props.blockAlign));
		
		bool success = AudioFileWriter::WriteWAVFileHeader(filePath, props, size);

		if (success)
		{
			const unsigned int dataSize = arraySize;
			const unsigned int fileSize = size + Audio::WAVTotalHeaderSize;
			unsigned int filePosition = Audio::WAVTotalHeaderSize;
			unsigned int dataIncrement = arraySize;
			track->SetStart(start);
			track->SetEnd(end);
			track->SetPosition(start);
			track->SetVolume(1.0f);
			while (filePosition < fileSize && success)
			{
				dataIncrement = (dataSize + filePosition) <= fileSize ? dataSize : (fileSize - filePosition);
				track->Read((byte*)data, dataIncrement);
				success = AudioFileWriter::WriteAudioFileData(filePath, data, dataIncrement);
				filePosition += dataIncrement;
			}
		}

		delete[arraySize] data;
		return success;
	}

	bool WAVFileBuilder::BuildWAVFile(Track* track, unsigned int start, unsigned int end, std::string filePath)
	{
		if (!track)
			return false;
		if (end <= start)
			return false;

		AudioProps props = track->GetProps();
		unsigned int arraySize = props.byteRate;
		char* data = new char[arraySize];
		unsigned int size = (end - (end % props.blockAlign)) - (start - (start % props.blockAlign));

		bool success = AudioFileWriter::WriteWAVFileHeader(filePath, props, size);

		if (success)
		{
			const unsigned int dataSize = arraySize;
			const unsigned int fileSize = size + Audio::WAVTotalHeaderSize;
			unsigned int filePosition = Audio::WAVTotalHeaderSize;
			unsigned int dataIncrement = dataSize;
			track->SetStart(start);
			track->SetEnd(end);
			track->SetPosition(start);
			track->SetVolume(1.0f);
			while (filePosition < fileSize && success)
			{
				dataIncrement = (dataSize + filePosition) <= fileSize ? dataSize : (fileSize - filePosition);
				track->Read((byte*)data, dataIncrement);
				success = AudioFileWriter::WriteAudioFileData(filePath, data, dataIncrement);
				filePosition += dataIncrement;
			}
		}

		delete[arraySize] data;
		return success;
	}
}