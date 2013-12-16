#include <RootSystems/Include/PlayerSystem.h>
#include <RootSystems/Include/Components.h>
#include <Utility/ECS/Include/World.h>

#include <RootEngine/Include/GameSharedContext.h>
extern RootEngine::GameSharedContext g_engineContext;

namespace RootForce
{
	void PlayerSystem::CreatePlayer()
	{
		ECS::EntityManager* entityManager = m_world->GetEntityManager();
		ECS::Entity* entity = entityManager->CreateEntity();
		
		RootForce::Renderable* renderable = entityManager->CreateComponent<RootForce::Renderable>(entity);
		RootForce::Transform* transform = entityManager->CreateComponent<RootForce::Transform>(entity);
		RootForce::PlayerControl* playerControl = entityManager->CreateComponent<RootForce::PlayerControl>(entity);
		RootForce::Player* player = entityManager->CreateComponent<RootForce::Player>(entity);
		RootForce::Physics* physics = entityManager->CreateComponent<RootForce::Physics>(entity);
		RootForce::Collision* collision = entityManager->CreateComponent<RootForce::Collision>(entity);
		RootForce::CollisionResponder* collisionRespond = entityManager->CreateComponent<RootForce::CollisionResponder>(entity);

		renderable->m_model = g_engineContext.m_resourceManager->LoadCollada("testchar");
		renderable->m_material.m_diffuseMap = g_engineContext.m_resourceManager->LoadTexture("WStexture", Render::TextureType::TEXTURE_2D);
		renderable->m_material.m_normalMap = g_engineContext.m_resourceManager->LoadTexture("WSSpecular", Render::TextureType::TEXTURE_2D);
		renderable->m_material.m_specularMap = g_engineContext.m_resourceManager->LoadTexture("WSNormal", Render::TextureType::TEXTURE_2D);
		renderable->m_material.m_effect = g_engineContext.m_resourceManager->LoadEffect("Mesh_NormalMap");

		playerControl->m_mouseSensitivity = 0.3f;
		playerControl->m_speed = 0.1f;

		player->m_abilities[0] = Abilitiy::ABILITY_TEST;
		player->m_selectedAbility = Abilitiy::ABILITY_TEST;

		physics->m_mass = 5.0f;
		collision->m_meshHandle = "testchar0";
		collision->m_handle = g_engineContext.m_physics->AddPlayerObjectToWorld(collision->m_meshHandle , entity->GetId(),
			transform->m_position, transform->m_orientation.GetQuaternion(), physics->m_mass, 10.0f, 0.0f, 0.1f, &collisionRespond->m_collidedEntityId);

		m_world->GetTagManager()->RegisterEntity("Player", entity);

		//Create player aiming device
		ECS::Entity* aimingDevice = entityManager->CreateEntity();
		m_world->GetTagManager()->RegisterEntity("AimingDevice", aimingDevice);

		RootForce::Transform* aimingDeviceTransform = entityManager->CreateComponent<RootForce::Transform>(aimingDevice);

	}

	void PlayerSystem::Process()
	{

	}
}