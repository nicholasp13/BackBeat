#include "bbpch.h"

#include "WAVFileBuilder.h"
#include "BackBeat/Core/FileDialog.h"
namespace BackBeat {

	bool WAVFileBuilder::BuildWAVFile(Track* track, unsigned int start, unsigned int end)
	{
		if (!track)
			return false;
		if (end <= start)
			return false;

		const unsigned int arraySize = 5000;
		char data[arraySize] = {};

		std::string filePath = FileDialog::SaveFile("WAV Files (*.wav)\0*.wav\0");
		AudioProps props = track->GetProps();
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
				track->Render((byte*)data, dataIncrement);
				success = AudioFileWriter::WriteAudioFileData(filePath, data, dataIncrement);
				filePosition += dataIncrement;
			}
		}
		return success;
	}

	bool WAVFileBuilder::BuildWAVFile(Track* track, unsigned int start, unsigned int end, std::string filePath)
	{
		if (!track)
			return false;
		if (end <= start)
			return false;

		const unsigned int arraySize = 5000;
		char data[arraySize] = {};
		AudioProps props = track->GetProps();
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
				track->Render((byte*)data, dataIncrement);
				success = AudioFileWriter::WriteAudioFileData(filePath, data, dataIncrement);
				filePosition += dataIncrement;
			}
		}
		return success;
	}
}