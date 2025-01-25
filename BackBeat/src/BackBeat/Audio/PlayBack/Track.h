#pragma once

#include "BackBeat/Audio/Audio.h"
namespace BackBeat {

	class Track
	{
	public:
		Track(AudioInfo info);
		~Track();

		virtual bool Read(byte* output, unsigned int numBytes);	
		virtual bool Write(byte* output, unsigned int numBytes);

		// Following functions: Reset and Clear, have the same functionality in this class but
		// are declared as different functions as they will have different implementations depending on 
		// the derived class. This is mostly becuase the basic Track class has no real way of overwriting
		// files using the standard c++ file i/o.
		virtual void Reset();
		virtual void Reset(AudioProps props);
		virtual void Clear();

		virtual TimeMinSec GetTime();
		virtual TimeMinSec GetTimeMs();
		virtual TimeMinSec GetLength();
		virtual void SetPosition(unsigned int position);
		virtual void SetStart(unsigned int start);
		virtual void SetEnd(unsigned int end);

		inline virtual bool IsDone() { return m_Done; }
		inline virtual unsigned int GetSize() { return m_Info.dataSize; }
		inline virtual unsigned int GetPosition() { return m_Position - m_Info.dataZero; }
		inline virtual unsigned int GetStart() { return m_StartPosition - m_Info.dataZero; }
		inline virtual unsigned int GetEnd() { return m_EndPosition - m_Info.dataZero; }
		inline virtual std::string GetName() { return m_Info.name; }
		inline virtual std::string GetFilePath() { return m_Info.filePath; }
		inline virtual AudioProps GetProps() { return m_Info.props; }
		inline virtual AudioInfo GetInfo() { return m_Info; }
		inline virtual FileType GetFileType() { return m_Info.type; }
		inline virtual void SetVolume(float vol) { m_Volume = vol; }
		inline virtual void SetName(std::string name) { m_Info.name = name; }
		inline virtual void SetDataSize(unsigned int size) { m_Info.dataSize = size; } // TODO: See if delete this

	private:
		bool m_Done;
		unsigned int m_Position;
		unsigned int m_StartPosition;
		unsigned int m_EndPosition;
		float m_Volume;
		AudioInfo m_Info;

	};
}