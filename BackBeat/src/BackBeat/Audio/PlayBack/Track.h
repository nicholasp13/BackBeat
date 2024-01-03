#pragma once

#include "BackBeat/Audio/Audio.h"
namespace BackBeat {

	class Track
	{
	public:
		Track(AudioInfo info);
		~Track();

		bool Render(byte* output, unsigned int numBytes);
		
		TimeMinSec GetTime();
		TimeMinSec GetLength();
		float GetProgress();
		void SetPosition(unsigned int position);
		void SetStart(unsigned int start);
		void SetEnd(unsigned int end);

		inline bool IsDone() { return m_Done; }
		inline unsigned int GetSize() { return m_Info.dataSize; }
		inline unsigned int GetPosition() { return m_Position - m_Info.dataZero; }
		inline unsigned int GetStart() { return m_StartPosition - m_Info.dataZero; }
		inline unsigned int GetEnd() { return m_EndPosition - m_Info.dataZero; }
		inline std::string GetName() { return m_Info.name; }
		inline AudioProps GetProps() { return m_Info.props; }
		inline AudioInfo GetInfo() { return m_Info; }
		inline void SetVolume(float vol) { m_Volume = vol; }

	private:
		bool m_Done;
		unsigned int m_Position;
		unsigned int m_StartPosition;
		unsigned int m_EndPosition;
		float m_Volume;
		AudioInfo m_Info;

		void MultiplyVolume(byte* output, unsigned int numBytes);
	};
}