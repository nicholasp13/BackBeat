#pragma once

#include "Track.h"
#include "BackBeat/File/FileMap.h"
namespace BackBeat {

	class MappedTrack : public Track
	{
	public:
		MappedTrack(AudioInfo info, std::shared_ptr<FileMap> fileMap);
		~MappedTrack();

		// MappedTrack functions
		bool Read(byte* output, unsigned int numBytes, unsigned int position);
		bool Write(byte* input, unsigned int numBytes, unsigned int position);

		void SetReadPosition(unsigned int position);
		void SetWritePosition(unsigned int position);

		inline unsigned int GetReadPosition() { return m_ReadPosition; }
		inline unsigned int GetWritePosition() { return m_ReadPosition; }

		// Track functions
		virtual bool Read(byte* output, unsigned int numBytes) override;
		virtual bool Write(byte* input, unsigned int numBytes) override;

		virtual void Reset() override;
		virtual void Reset(AudioProps props) override;
		virtual void Clear() override;

		virtual TimeMinSec GetTime() override;
		virtual TimeMinSec GetTimeMs() override;
		virtual TimeMinSec GetLength() override;
		virtual void SetPosition(unsigned int position) override;
		virtual void SetStart(unsigned int start) override;
		virtual void SetEnd(unsigned int end) override;

		inline virtual bool IsDone() override { return m_Done; }
		inline virtual unsigned int GetSize() override { return m_Info.dataSize; }
		inline virtual unsigned int GetPosition() override { return m_ReadPosition - m_Info.dataZero; }
		inline virtual unsigned int GetStart() override { return m_StartPosition - m_Info.dataZero; }
		inline virtual unsigned int GetEnd() override { return m_EndPosition - m_Info.dataZero; }
		inline virtual std::string GetName() override { return m_Info.name; }
		inline virtual std::string GetFilePath() override { return m_Info.filePath; }
		inline virtual AudioProps GetProps() override { return m_Info.props; }
		inline virtual AudioInfo GetInfo() override { return m_Info; }
		inline virtual FileType GetFileType() override { return m_Info.type; }
		inline virtual void SetVolume(float vol) override { m_Volume = vol; }
		inline virtual void SetName(std::string name) override { m_Info.name = name; }

	private:
		bool m_Done;
		unsigned int m_ReadPosition;
		unsigned int m_WritePosition;
		unsigned int m_StartPosition;
		unsigned int m_EndPosition;
		float m_Volume;

		AudioInfo m_Info;
		std::shared_ptr<FileMap> m_FileMap;

	};

}