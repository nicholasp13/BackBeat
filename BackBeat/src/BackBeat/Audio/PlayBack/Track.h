#pragma once

#include "BackBeat/Audio/Audio.h"
#include "AudioData.h"
namespace BackBeat {

	class Track
	{
	public:
		Track(AudioData* data);
		~Track();

		bool Render(byte* output, unsigned int numBytes);
		
		TimeMinSec GetTime();
		TimeMinSec GetLength();
		void SetPosition(unsigned int position);

		float GetProgress();

		bool IsDone() { return m_Done; }
		unsigned int GetSize() { return m_Data->GetDataSize(); }
		unsigned int GetPosition() { return m_Position - m_Data->GetZero(); }
		std::string GetName() { return m_Data->GetName(); }
		AudioProps GetProps() { return m_Data->GetProps(); }
		void SetVolume(float vol) { m_Volume = vol; }

	private:
		bool m_Done;
		unsigned int m_Position;
		float m_Volume;
		AudioData* m_Data;

		void MultiplyVolume(byte* output, unsigned int numBytes);
	};
}