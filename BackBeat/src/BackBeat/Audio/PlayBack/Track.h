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

		bool IsDone() { return m_Done; }
		unsigned int GetSize() { return m_Info.dataSize; }
		unsigned int GetPosition() { return m_Position - m_Info.dataZero; }
		unsigned int GetStart() { return m_StartPos - m_Info.dataZero; }
		unsigned int GetEnd() { return m_EndPos - m_Info.dataSize; }
		std::string GetName() { return m_Info.name; }
		AudioProps GetProps() { return m_Info.props; }
		AudioInfo GetInfo() { return m_Info; }

		void SetVolume(float vol) { m_Volume = vol; }

	private:
		bool m_Done;
		unsigned int m_Position;
		unsigned int m_StartPos;
		unsigned int m_EndPos;
		float m_Volume;
		AudioInfo m_Info;

		void MultiplyVolume(byte* output, unsigned int numBytes);
	};
}