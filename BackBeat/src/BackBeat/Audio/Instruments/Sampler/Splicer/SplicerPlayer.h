#pragma once

#include "SplicerPlayerProcessor.h"
namespace BackBeat {

	class SplicerPlayer
	{
	public:
		SplicerPlayer();
		~SplicerPlayer();

		void Start();
		void Stop();
		void Load(float* leftChannel, float* rightChannel, unsigned int numFrames);
		void Clear();
		
		inline void Play() { m_Proc->On(); }
		inline void Pause() { m_Proc->Off(); }
		inline void On() { m_Proc->On(); }
		inline void Off() { m_Proc->Off(); }
		inline bool IsOn() { return m_Proc->IsOn(); }
		inline void LoopOn() { m_Proc->LoopOn(); }
		inline void LoopOff() { m_Proc->LoopOff(); }
		inline bool IsLooping() { return m_Proc->IsLooping(); }
		inline bool IsLoaded() { return m_TotalFrames > 0; }
		inline std::shared_ptr<SplicerPlayerProcessor> GetProc() { return m_Proc; }

	private:
		unsigned int m_TotalFrames;
		AudioProps m_Props;

		// Shared ptr of audio proc
		std::shared_ptr<SplicerPlayerProcessor> m_Proc;
	};

}