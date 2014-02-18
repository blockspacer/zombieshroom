#include "UnitTesting.h"
#include <RootSystems/Include/AbilityRespawnSystem.h>
#include <RootSystems/Include/PlayerSystem.h>
#include <RootEngine/Script/Include/RootScript.h>

TEST(AbilitySpawn, ProcessEmptyEntity)
{
	ECS::World* world = CreateWorld();
	g_world = world;
	g_networkEntityMap.clear();

	ECS::Entity* testity = world->GetEntityManager()->CreateEntity();
	RootForce::AbilityRespawnSystem* system = new RootForce::AbilityRespawnSystem(world, &g_engineContext, g_engineContext.m_resourceManager->GetWorkingDirectory());
	world->GetSystemManager()->AddSystem<RootForce::AbilityRespawnSystem>(system);
	// Will test that an empty entity (an entity missing the necessary components does not crash the system
	system->Process();
	world->GetEntityManager()->RemoveAllEntitiesAndComponents();
	world->GetTagManager()->UnregisterAll();
	world->GetGroupManager()->UnregisterAll();
	g_engineContext.m_physics->RemoveAll();
	delete world;
}

TEST(AbilitySpawn, LoadAndAttatch)
{
	ECS::World* world = CreateWorld();
	g_world = world;
	g_networkEntityMap.clear();

	ECS::Entity* testity = world->GetEntityManager()->CreateEntity();
	world->GetGroupManager()->RegisterEntity("AbilitySpawnPoint", testity);
	RootForce::AbilityRespawnSystem* system = new RootForce::AbilityRespawnSystem(world, &g_engineContext, g_engineContext.m_resourceManager->GetWorkingDirectory());
	world->GetSystemManager()->AddSystem<RootForce::AbilityRespawnSystem>(system);
	g_engineContext.m_resourceManager->LoadScript("AbilityBall");
	g_engineContext.m_resourceManager->LoadScript("AbilitySpawnPoint");
	//Test so that loading an ability pack works
	{
		system->LoadAbilities("Fake"); //Load a fake pack, should give a logg message but no crash

		system->LoadAbilities("Standard"); //Load existing pack
	}

	{
		system->AttatchComponentToPoints();

		RootForce::AbilityRespawnComponent* component = world->GetEntityManager()->GetComponent<RootForce::AbilityRespawnComponent>(testity);
		EXPECT_NE(component, nullptr); //Make sure that the component was attached correctly by checking so that it is not null
	}

	world->GetEntityManager()->RemoveAllEntitiesAndComponents();
	world->GetTagManager()->UnregisterAll();
	world->GetGroupManager()->UnregisterAll();
	g_engineContext.m_physics->RemoveAll();
	delete world;
}

TEST(AbilitySpawn, ProcessEntity)
{
	ECS::World* world = CreateWorld();
	g_world = world;
	g_networkEntityMap.clear();

	ECS::Entity* testity = world->GetEntityManager()->CreateEntity();
	world->GetGroupManager()->RegisterEntity("AbilitySpawnPoint", testity);
	RootForce::AbilityRespawnSystem* system = new RootForce::AbilityRespawnSystem(world, &g_engineContext, g_engineContext.m_resourceManager->GetWorkingDirectory());
	world->GetSystemManager()->AddSystem<RootForce::AbilityRespawnSystem>(system);

	g_engineContext.m_resourceManager->LoadScript("AbilityBall");
	g_engineContext.m_resourceManager->LoadScript("AbilitySpawnPoint");

	system->LoadAbilities("Standard"); //Load existing pack
	system->AttatchComponentToPoints();

	RootForce::Renderable* rendcomp;
	RootForce::AbilityRespawnComponent* spawncomp;

	{
		system->Process();
		rendcomp = world->GetEntityManager()->GetComponent<RootForce::Renderable>(testity);
		spawncomp = world->GetEntityManager()->GetComponent<RootForce::AbilityRespawnComponent>(testity);
		EXPECT_NE(rendcomp, nullptr); //Make sure that a new render component was created in the process since the point had no active ability

		EXPECT_NE(spawncomp->CurrentAbility.Name, ""); //Check that we got an ability to spawn, if the name is empty there is no ability
	}

	{
		spawncomp->Claimed = true;
		system->Process();
		rendcomp = world->GetEntityManager()->GetComponent<RootForce::Renderable>(testity);
		EXPECT_EQ(spawncomp->Timer, 30.0f); //Check that the timer has been set after the ability was claimed
		EXPECT_EQ(spawncomp->CurrentAbility.Name, ""); //Check that there is no ability present at the point
		EXPECT_EQ(rendcomp, nullptr); //Check that the render component has been removed correctly
	}

	world->GetEntityManager()->RemoveAllEntitiesAndComponents();
	world->GetTagManager()->UnregisterAll();
	world->GetGroupManager()->UnregisterAll();
	g_engineContext.m_physics->RemoveAll();
	delete world;
}