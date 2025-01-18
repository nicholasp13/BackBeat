#pragma once

#include "Track.h"
#include "BackBeat/File/FileMap.h"
namespace BackBeat {

	class MappedTrack : public Track
	{
	public:
		MappedTrack(AudioInfo info, std::shared_ptr<FileMap> fileMap);
		~MappedTrack();

		virtual bool Read(byte* output, unsigned int numBytes) override;
		virtual bool Write(byte* input, unsigned int numBytes) override;

		virtual TimeMinSec GetTime() override;
		virtual TimeMinSec GetLength() override;
		virtual void SetPosition(unsigned int position) override;
		virtual void SetStart(unsigned int start) override;
		virtual void SetEnd(unsigned int end) override;

		inline virtual bool IsDone() override { return m_Done; }
		inline virtual unsigned int GetSize() override { return m_Info.dataSize; }
		inline virtual unsigned int GetPosition() override { return m_Position - m_Info.dataZero; }
		inline virtual unsigned int GetStart() override { return m_StartPosition - m_Info.dataZero; }
		inline virtual unsigned int GetEnd() override { return m_EndPosition - m_Info.dataZero; }
		inline virtual std::string GetName() override { return m_Info.name; }
		inline virtual std::string GetFilePath() override { return m_Info.filePath; }
		inline virtual AudioProps GetProps() override { return m_Info.props; }
		inline virtual AudioInfo GetInfo() override { return m_Info; }
		inline virtual FileType GetFileType() override { return m_Info.type; }
		inline virtual void SetVolume(float vol) override { m_Volume = vol; }
		inline virtual void SetDataSize(unsigned int size) override { m_Info.dataSize = size; } // NOTE: This should ONLY be used by the Recording class

	private:
		bool m_Done;
		unsigned int m_Position;
		unsigned int m_StartPosition;
		unsigned int m_EndPosition;
		float m_Volume;

		AudioInfo m_Info;
		std::shared_ptr<FileMap> m_FileMap;

	};

}