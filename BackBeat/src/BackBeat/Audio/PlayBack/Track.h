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
		void SetPosition(unsigned int position);

		float GetProgress();

		bool IsDone() { return m_Done; }
		unsigned int GetSize() { return m_Info.dataSize; }
		unsigned int GetPosition() { return m_Position - m_Info.dataZero; }
		std::string GetName() { return m_Info.name; }
		AudioProps GetProps() { return m_Info.props; }
		AudioInfo GetInfo() { return m_Info; }
		void SetVolume(float vol) { m_Volume = vol; }

	private:
		bool m_Done;
		unsigned int m_Position;
		float m_Volume;
		AudioInfo m_Info;

		void MultiplyVolume(byte* output, unsigned int numBytes);
	};
}