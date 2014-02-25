#pragma once

#include <RootEngine/Include/GameSharedContext.h>
#include <RootSystems/Include/PhysicsSystem.h>
#include <Utility/ECS/Include/Component.h>
#include <Utility/ECS/Include/EntitySystem.h>
#include <Utility/ECS/Include/World.h>

namespace RootForce
{
	struct HomingComponent : public ECS::Component<HomingComponent>
	{
		ECS::Entity* TargetPlayer;
		glm::vec3 TargetPosition;
		float Controllability;
		HomingComponent()
			: TargetPlayer(nullptr),
			  TargetPosition(glm::vec3(0,0,0)),
			  Controllability(0.0f)
		{}
	};

	class HomingSystem : public ECS::EntitySystem
	{
	public:
		HomingSystem(ECS::World* p_world/*, RootEngine::GameSharedContext* p_engineContext, std::string p_workingDir*/)
			: ECS::EntitySystem(p_world)
			//, m_engineContext(p_engineContext)
		{ 
			SetUsage<RootForce::HomingComponent>();
			SetUsage<RootForce::Collision>();
		}
		void Init();
		void ProcessEntity(ECS::Entity* p_entity);

	private:


		RootEngine::GameSharedContext* m_engineContext;
		ECS::ComponentMapper<RootForce::Collision> m_physics;
		ECS::ComponentMapper<RootForce::HomingComponent> m_target;
	};
}