#pragma once

#include <RootEngine/Network/Include/NetworkManager.h>
#include <RootSystems/Include/Network/ClientMessageSystem.h>
#include <RootSystems/Include/Network/ServerMessageSystem.h>
#include <RootSystems/Include/Network/NetworkComponents.h>
#include <RootSystems/Include/Network/NetworkEntityMap.h>

namespace RootForce
{
	namespace Network
	{
		class MessageHandler
		{
		public:
			enum ServerType
			{
				LOCAL,
				REMOTE,
				DEDICATED
			};

			MessageHandler(ECS::World* p_world, Logging* p_logger, RootEngine::Network::NetworkInterface* p_networkInterface, ServerType p_type, int16_t port, const char* address = "");

			void Update();
			void SetChatSystem(ChatSystemInterface* p_chatSystem);

		private:
			ECS::World* m_world;
			Logging* m_logger;

			RootEngine::Network::Server* m_server;
			ClientMessageHandler* m_clientMessageHandler;
			ServerMessageHandler* m_serverMessageHandler;
		};
	}
}