#include "bbpch.h"

#include "AudioFileBuilder.h"
#include "BackBeat/Core/FileDialog.h"
namespace BackBeat {

	bool AudioFileBuilder::BuildWAVFile(Track* track, unsigned int start, unsigned int end)
	{
		if (!track)
			return false;
		if (end <= start)
			return false;

		std::string filePath = FileDialog::SaveFile("WAV Files (*.wav)\0*.wav\0");
		AudioProps props = track->GetProps();
		unsigned int size = (end - (end % props.blockAlign)) - (start - (start % props.blockAlign));
		
		bool success = AudioFileWriter::WriteWAVFileHeader(filePath, props, size);

		if (success)
		{
			const unsigned int dataSize = props.byteRate;
			const unsigned int fileSize = size + Audio::WAVTotalHeaderSize;
			unsigned int filePosition = Audio::WAVTotalHeaderSize;
			unsigned int dataIncrement = dataSize;
			char* data = new char[dataSize];
			track->SetStart(start);
			track->SetEnd(end);
			track->SetPosition(start);
			track->SetVolume(1.0f);
			while (filePosition < fileSize && success)
			{
				dataIncrement = (dataSize + filePosition) <= fileSize ? dataSize : (fileSize - filePosition);
				track->Render((byte*)data, dataIncrement);
				success = AudioFileWriter::WriteWAVFileData(filePath, data, dataIncrement, filePosition);
				filePosition += dataIncrement;
			}
			delete[size] data;
		}
		return success;
	}
}