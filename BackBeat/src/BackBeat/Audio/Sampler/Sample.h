#pragma once

#include "BackBeat/Audio/Audio.h"
namespace BackBeat {

	class Sample
	{
	public:
		Sample(AudioInfo info);
		~Sample();

		bool Render(byte* output, unsigned int numBytes);
		void Off();
		void Reset();

		inline void On() { m_Done = false; }
		inline bool IsDone() { return m_Done; }
		inline bool IsActive() { return !m_Done && m_Position < m_Info.dataSize + m_Info.dataZero; }
		inline unsigned int GetBytePosition() { return m_Position - m_Info.dataZero; }
		inline unsigned int GetByteSize() { return m_Info.dataSize; }
		inline AudioInfo GetInfo() { return m_Info; }
		inline std::string GetName() { return m_Info.name; }
		inline AudioProps GetProps() { return m_Info.props; }

	private:
		bool m_Done;
		unsigned int m_Position;
		AudioInfo m_Info;

	};
}