#pragma once
#include <RootEngine/Include/GameSharedContext.h>
#include <Utility/ECS/Include/World.h>
#include <RootSystems/Include/WorldSystem.h>
#include <RootSystems/Include/RenderingSystem.h>
#include <RootSystems/Include/LightSystem.h>
#include <RootSystems/Include/PlayerSystem.h>
#include <RootSystems/Include/PlayerControlSystem.h>
#include <RootSystems/Include/AbilitySystem.h>
#include <RootSystems/Include/CameraSystem.h>
#include <RootSystems/Include/PhysicsSystem.h>
#include <RootForce/Include/ComponentExporter.h>
#include <RootForce/Include/ComponentImporter.h>
#include <RootSystems/Include/Components.h>

#include <RootForce/Include/GameStates.h>
#include <RootSystems/Include/Network/Server.h>
#include <RootSystems/Include/Network/Client.h>
#include <RootSystems/Include/Network/MessageHandlers.h>
#include <RootSystems/Include/ChatSystem.h>
#include <RootForce/Include/HUD.h>

namespace RootForce
{
	class Ingamestate
	{
	public:
		Ingamestate();
		void Initialize(RootEngine::GameSharedContext* p_engineContext, 
			ECS::World* p_world, 
			GameStates::PlayData p_playData, 
			RootForce::Network::Client* p_client, 
			RootForce::Network::ClientMessageHandler* p_clientMessageHandler);
		void Update(float p_deltaTime);
	private:
		RootEngine::GameSharedContext* m_engineContext;

		ECS::World* m_world;

		std::shared_ptr<RootForce::Network::Server> m_server;
		std::shared_ptr<RootForce::Network::ServerMessageHandler> m_serverMessageHandler;
		RootForce::Network::Client* m_client;
		RootForce::Network::ClientMessageHandler* m_clientMessageHandler;

		bool m_displayNormals; // TODO: May not be needed?
		bool m_displayPhysicsDebug; // TODO: May not be needed?

		std::shared_ptr<RootForce::HUD> m_hud;

		//Game systems
		std::shared_ptr<RootForce::PlayerControlSystem> m_playerControlSystem;
		std::shared_ptr<RootForce::WorldSystem> m_worldSystem;
		std::shared_ptr<RootForce::PlayerSystem> m_playerSystem;
		RootForce::ScriptSystem* m_scriptSystem;
		RootForce::PhysicsSystem* m_physicsSystem;
		RootForce::CollisionSystem* m_collisionSystem;
		RootForce::RenderingSystem* m_renderingSystem;
		RootForce::PointLightSystem* m_pointLightSystem;
		RootForce::CameraSystem* m_cameraSystem;
		RootForce::LookAtSystem* m_lookAtSystem;
		RootForce::ThirdPersonBehaviorSystem* m_thirdPersonBehaviorSystem;
	};
}