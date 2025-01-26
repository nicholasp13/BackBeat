#pragma once

#include <BackBeat.h>

#include "Entity/Entity.h"
namespace Exampler {

	class Canvas
	{
	public:
		Canvas();
		~Canvas();

		void Init(BackBeat::AudioProps props, BackBeat::PlayerManager* playerMgr, BackBeat::RecorderManager* recorderMgr);
		void Render(float width, float height, bool active);
		void AddEntity(std::shared_ptr<Entity> entity);
		void DeleteEntity(std::shared_ptr<Entity> entity);
		void Reset();

	private:
		static const unsigned long s_BufferSize = 480000; // This is about 10 secondsd for 1 channel of a stereo 48k sample rate
		                                                  // floating point AudioProps

		unsigned int m_Position;
		float m_Width;
		float m_Height;
		std::shared_ptr<float[]> m_Buffer;
		std::vector< std::shared_ptr<Entity> > m_Entities;

		// BackBeat members
		BackBeat::AudioProps m_Props;
		BackBeat::AudioFileLoader m_Loader;
		BackBeat::PlayerManager* m_PlayerMgr;
		BackBeat::RecorderManager* m_RecorderMgr;

	private:
		void RenderEntities();
		unsigned int SetColors();

	};

}