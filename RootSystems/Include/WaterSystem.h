#pragma once

#include <map>
#include <RootEngine/Include/ModelImporter.h>
#include <RootEngine/Include/Logging/Logging.h>
#include <Utility/ECS/Include/World.h>
#include <RootSystems/Include/RenderingSystem.h>
#include <RootEngine/Include/GameSharedContext.h>
namespace RootForce
{
	struct WaterSystem : public ECS::VoidSystem
	{
		WaterSystem(ECS::World* p_world, RootEngine::GameSharedContext* p_context)
			: ECS::VoidSystem(p_world), m_context(p_context), m_world(p_world)
		{
		}
		void Init();
		void Begin();
		void Process();
		void End();
		void CreateRenderable();

		
	private:
		Render::Material*						m_material;				
		Render::TextureInterface*				m_texture[2];
		Logging::LoggingInterface*				m_logger;
		RootEngine::GameSharedContext*			m_context;
		Render::ComputeJob						m_computeJob;
		ECS::World*								m_world;

		unsigned m_currTex;
		float m_mk1, m_mk2, m_mk3;
	};
}