#pragma once

#include <BackBeat.h>

#include "Entity/Entity.h"
namespace Exampler {

	class Canvas
	{
	public:
		Canvas();
		~Canvas();

		void Init(BackBeat::PlayerManager* playerMgr);
		void Render(float width, float height, bool active);
		void AddEntity(std::shared_ptr<Entity> entity);
		void DeleteEntity(std::shared_ptr<Entity> entity);

	private:
		float m_Width;
		float m_Height;
		std::vector< std::shared_ptr<Entity> > m_Entities;

		// BackBeat members
		BackBeat::PlayerManager* m_PlayerMgr;

	private:
		void RenderEntities();
		unsigned int SetColors();
	};

}