#include "UnitTesting.h"
#include <RootSystems/Include/StateSystem.h>
#include <RootSystems/Include/ActionSystem.h>
#include <RootSystems/Include/PlayerSystem.h>
#include <RootEngine/Script/Include/RootScript.h>

TEST(StateSystem, ProcessEmptyEntity)
{
	ECS::World* world = CreateWorld();
	g_world = world;
	g_networkEntityMap.clear();

	ECS::Entity* testity = world->GetEntityManager()->CreateEntity();
	RootSystems::StateSystem* system = new RootSystems::StateSystem(world, &g_engineContext);
	world->GetSystemManager()->AddSystem<RootSystems::StateSystem>(system);
	// Will test that an empty entity (an entity missing the necessary components does not crash the system
	system->Process();
	world->GetEntityManager()->RemoveAllEntitiesAndComponents();
	world->GetTagManager()->UnregisterAll();
	world->GetGroupManager()->UnregisterAll();
	g_engineContext.m_physics->RemoveAll();
	delete world;
}

TEST(StateSystem, ProcessEntity)
{
	ECS::World* world = CreateWorld();
	g_world = world;
	g_networkEntityMap.clear();

	RootSystems::StateSystem* system = new RootSystems::StateSystem(world, &g_engineContext);
	world->GetSystemManager()->AddSystem<RootSystems::StateSystem>(system);
	RootForce::PhysicsSystem* pSystem = new RootForce::PhysicsSystem(world); 
	pSystem->SetPhysicsInterface(g_engineContext.m_physics);
	pSystem->SetLoggingInterface(g_engineContext.m_logger);
	world->GetSystemManager()->AddSystem<RootForce::PhysicsSystem>(pSystem);
	RootSystems::ActionSystem* aSystem = new RootSystems::ActionSystem(world, &g_engineContext);
	world->GetSystemManager()->AddSystem<RootSystems::ActionSystem>(aSystem);

	// Call the OnCreate script
	g_engineContext.m_script->SetGlobalNumber("UserID", 0);
	g_engineContext.m_script->SetFunction(g_engineContext.m_resourceManager->LoadScript("Player"), "OnCreate");
	//g_engineContext.m_script->AddParameterUserData(testity, sizeof(ECS::Entity*), "Entity");
	g_engineContext.m_script->AddParameterNumber(0);
	g_engineContext.m_script->AddParameterNumber(0);
	g_engineContext.m_script->ExecuteScript();
	
	ECS::Entity* testity = world->GetTagManager()->GetEntityByTag("Player");

	RootForce::StateComponent* state = world->GetEntityManager()->GetComponent<RootForce::StateComponent>(testity);
	RootForce::Transform* transform = world->GetEntityManager()->GetComponent<RootForce::Transform>(testity);
	RootForce::PlayerActionComponent* action = world->GetEntityManager()->GetComponent<RootForce::PlayerActionComponent>(testity);
	g_engineContext.m_physics->CreatePlane(glm::vec3(0,1,0),glm::vec3(0,-50,0)); 



	{
		state->PrevPosition = transform->m_position;
		float y = transform->m_position.y;
		world->SetDelta(0.1f);

		aSystem->Process();
		g_engineContext.m_physics->Update(0.1f);
		pSystem->Process();
		system->Process();
		EXPECT_LT(transform->m_position.y, y);
		EXPECT_EQ(state->CurrentState, RootForce::EntityState::DESCENDING);
	}

	//Make sure we are not falling when testing the positioning
	for(int i = 0; i < 100; i++)
	{
		aSystem->Process();
		g_engineContext.m_physics->Update(0.1f);
		pSystem->Process();
		system->Process();
	}

	{
		EXPECT_EQ(state->CurrentState, RootForce::EntityState::GROUNDED);
		action->JumpTime = 1.0f;
		aSystem->Process();
		g_engineContext.m_physics->Update(0.1f);
		pSystem->Process();
		system->Process();
		EXPECT_EQ(state->CurrentState, RootForce::EntityState::ASCENDING);
	}
	world->GetEntityManager()->RemoveAllEntitiesAndComponents();
	world->GetTagManager()->UnregisterAll();
	world->GetGroupManager()->UnregisterAll();
	g_engineContext.m_physics->RemoveAll();
	delete world;
}