#include <RootSystems/Include/Network/MessageHandlers.h>
#include <RootSystems/Include/Network/ServerInfo.h>
#include <RootSystems/Include/Network/Messages.h>
#include <RootSystems/Include/Network/NetworkComponents.h>
#include <RootSystems/Include/Components.h>
#include <cassert>

extern RootEngine::GameSharedContext g_engineContext;

namespace RootForce
{
	namespace Network
	{
		MessageHandler::MessageHandler(RakNet::RakPeerInterface* p_peer)
			: m_peer(p_peer) {}
		MessageHandler::~MessageHandler() {}


		ClientMessageHandler::ClientMessageHandler(RakNet::RakPeerInterface* p_peer, ECS::World* p_world)
			: MessageHandler(p_peer)
			, m_world(p_world)
			, m_networkEntityMap(nullptr)
			, m_list(nullptr)
			, m_chatSystem(nullptr)
			, m_worldSystem(nullptr) {}


		void ClientMessageHandler::SetNetworkEntityMap(NetworkEntityMap* p_networkEntityMap)
		{
			m_networkEntityMap = p_networkEntityMap;
		}

		void ClientMessageHandler::SetLanList(RootSystems::LanList* p_list)
		{
			m_list = p_list;
		}

		void ClientMessageHandler::SetChatSystem(RootForce::ChatSystem* p_chatSystem)
		{
			m_chatSystem = p_chatSystem;
		}

		void ClientMessageHandler::SetWorldSystem(WorldSystem* p_worldSystem)
		{
			m_worldSystem = p_worldSystem;
		}

		bool ClientMessageHandler::ParsePacket(RakNet::MessageID p_id, RakNet::BitStream* p_bs, RakNet::Packet* p_packet)
		{
			ECS::Entity* clientEntity = m_world->GetTagManager()->GetEntityByTag("Client");
			Network::ClientComponent* clientComponent = m_world->GetEntityManager()->GetComponent<Network::ClientComponent>(clientEntity);

			NetworkEntityMap& networkEntityMap = *m_networkEntityMap;

			switch (p_id)
			{
				case ID_CONNECTION_REQUEST_ACCEPTED:
				{
					g_engineContext.m_logger->LogText(LogTag::CLIENT, LogLevel::SUCCESS, "Connection accepted");

					clientComponent->State = ClientState::AWAITING_SERVER_INFO;

					// Send user information
					NetworkMessage::UserInformation m;
					m.Name = clientComponent->Name;

					RakNet::BitStream bs;
					bs.Write((RakNet::MessageID) NetworkMessage::MessageType::UserInformation);
					m.Serialize(true, &bs);

					m_peer->Send(&bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, m_peer->GetSystemAddressFromIndex(0), false);
				} return true;

				case ID_NO_FREE_INCOMING_CONNECTIONS:
				{
					g_engineContext.m_logger->LogText(LogTag::CLIENT, LogLevel::WARNING, "Connection refused: server full");

					clientComponent->State = ClientState::DISCONNECTED_REFUSED_TOO_MANY_PLAYERS;
				} return true;

				case ID_DISCONNECTION_NOTIFICATION:
				{
					g_engineContext.m_logger->LogText(LogTag::CLIENT, LogLevel::WARNING, "Connection terminated: booted");

					clientComponent->State = ClientState::DISCONNECTED_SERVER_SHUTDOWN;
				} return true;

				case ID_CONNECTION_LOST:
				{
					g_engineContext.m_logger->LogText(LogTag::CLIENT, LogLevel::WARNING, "Connection terminated: connection to server lost");

					clientComponent->State = ClientState::DISCONNECTED_TIMEOUT;
				} return true;

				case ID_CONNECTION_ATTEMPT_FAILED:
				{
					g_engineContext.m_logger->LogText(LogTag::CLIENT, LogLevel::WARNING, "Connection attempt failed");

					clientComponent->State = ClientState::DISCONNECTED_REFUSED;
				} return true;

				case ID_UNCONNECTED_PONG:
				{
					p_bs->IgnoreBytes(4); // Timestamp
					
					RootSystems::ServerInfoInternal info;
					info.Information.Serialize(false, p_bs);
					info.IP = p_packet->systemAddress.ToString(false);
					info.Port = p_packet->systemAddress.GetPort();

					m_list->AddServer(info);
				} return true;

				case NetworkMessage::MessageType::GameStateDelta:
				{

				} return true;

				case NetworkMessage::MessageType::Chat:
				{
					NetworkMessage::Chat m;
					m.Serialize(false, p_bs);

					m_chatSystem->JSAddMessage(m.Message.C_String());
				} return true;

				case NetworkMessage::MessageType::UserConnected:
				{
					NetworkMessage::UserConnected m;
					m.Serialize(false, p_bs);

					NetworkEntityID id;
					id.UserID = m.User;
					id.ActionID = ReservedActionID::CONNECT;
					id.SequenceID = 0;

					if (clientComponent->IsRemote)
					{
						// If we are a remote client, we need to create our own entities.
						ECS::Entity* player = m_world->GetEntityManager()->CreateEntity();

						if (m.IsYou)
						{
							m_world->GetTagManager()->RegisterEntity("Player", player);
						}

						networkEntityMap[id] = player;

						// Call the OnCreate script
						g_engineContext.m_script->SetFunction(g_engineContext.m_resourceManager->GetScript("Player"), "OnCreate");
						g_engineContext.m_script->AddParameterUserData(player, sizeof(ECS::Entity*), "Entity");
						g_engineContext.m_script->AddParameterNumber(id.UserID);
						g_engineContext.m_script->AddParameterNumber(id.ActionID);
						g_engineContext.m_script->ExecuteScript();
					}

					// Add client components onto the entity
					g_engineContext.m_script->SetFunction(g_engineContext.m_resourceManager->GetScript("Player"), "AddClientComponents");
					g_engineContext.m_script->AddParameterUserData(networkEntityMap[id], sizeof(ECS::Entity*), "Entity");
					g_engineContext.m_script->ExecuteScript();

					if (m.IsYou)
					{
						// Add client components onto the entity
						g_engineContext.m_script->SetFunction(g_engineContext.m_resourceManager->GetScript("Player"), "AddLocalClientComponents");
						g_engineContext.m_script->AddParameterUserData(networkEntityMap[id], sizeof(ECS::Entity*), "Entity");
						g_engineContext.m_script->ExecuteScript();

						clientComponent->State = ClientState::CONNECTED;
					}

					g_engineContext.m_logger->LogText(LogTag::CLIENT, LogLevel::DEBUG_PRINT, "User connected (%s): %s", p_packet->systemAddress.ToString(), m.Name);
				} return true;

				case NetworkMessage::MessageType::UserDisconnected:
				{
					
				} return true;

				case NetworkMessage::MessageType::PlayerCommand:
				{
					
				} return true;

				case NetworkMessage::MessageType::DestroyEntities:
				{
					
				} return true;

				case NetworkMessage::MessageType::SpawnUser:
				{

				} return true;

				case NetworkMessage::MessageType::LoadMap:
				{

				} return true;

				case NetworkMessage::MessageType::SetMaxPlayers:
				{

				} return true;

				case NetworkMessage::MessageType::SetGameMode:
				{

				} return true;

				case NetworkMessage::MessageType::SetMatchTime:
				{

				} return true;

				case NetworkMessage::MessageType::SetKillCount:
				{

				} return true;

				case NetworkMessage::MessageType::ServerInformation:
				{
					// Local clients already have the map loaded by the local server.
					if (clientComponent->IsRemote)
					{
						ECS::Entity* serverInfoEntity = m_world->GetTagManager()->GetEntityByTag("ServerInformation");
						Network::ServerInformationComponent* serverInfo = m_world->GetEntityManager()->GetComponent<Network::ServerInformationComponent>(serverInfoEntity);

						serverInfo->Information.Serialize(false, p_bs);

						// If we are a remote client, set the rule entity information
						if (clientComponent->IsRemote)
						{
							ECS::Entity* ruleEntity = m_world->GetTagManager()->GetEntityByTag("MatchState");
							RootForce::TDMRuleSet* rules = m_world->GetEntityManager()->GetComponent<RootForce::TDMRuleSet>(ruleEntity);

							rules->ScoreLimit = serverInfo->Information.KillCount;
							rules->TimeLeft = (float) serverInfo->Information.MatchTimeSeconds;
						}

						// Load the map
						m_worldSystem->CreateWorld(serverInfo->Information.MapName.C_String());
					}

					// Send load map status
					NetworkMessage::LoadMapStatus m;
					m.Status = NetworkMessage::LoadMapStatus::STATUS_COMPLETED;
					
					RakNet::BitStream bs;
					bs.Write((RakNet::MessageID) NetworkMessage::MessageType::LoadMapStatus);
					m.Serialize(true, &bs);

					m_peer->Send(&bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, m_peer->GetSystemAddressFromIndex(0), false);

					// Set the new client state
					clientComponent->State = ClientState::AWAITING_USER_CONNECT;
				} return true;
			}

			return false;
		}



		ServerMessageHandler::ServerMessageHandler(RakNet::RakPeerInterface* p_peer, ECS::World* p_world)
			: MessageHandler(p_peer)
			, m_world(p_world)
			, m_networkEntityMap(nullptr) {}

		void ServerMessageHandler::SetNetworkEntityMap(NetworkEntityMap* p_networkEntityMap)
		{
			m_networkEntityMap = p_networkEntityMap;
		}

		bool ServerMessageHandler::ParsePacket(RakNet::MessageID p_id, RakNet::BitStream* p_bs, RakNet::Packet* p_packet)
		{
			ECS::Entity* serverInfoEntity = m_world->GetTagManager()->GetEntityByTag("ServerInformation");
			Network::ServerInformationComponent* serverInfo = m_world->GetEntityManager()->GetComponent<Network::ServerInformationComponent>(serverInfoEntity);

			NetworkEntityMap& networkEntityMap = *m_networkEntityMap;

			switch (p_id)
			{
				case ID_NEW_INCOMING_CONNECTION:
				{
					g_engineContext.m_logger->LogText(LogTag::SERVER, LogLevel::SUCCESS, "Client connected (%s)", p_packet->systemAddress.ToString());

					ECS::Entity* playerEntity = m_world->GetEntityManager()->CreateEntity();
					m_world->GetTagManager()->RegisterEntity("Player", playerEntity);
					
					NetworkEntityID id;
					id.UserID = m_peer->GetIndexFromSystemAddress(p_packet->systemAddress);
					id.ActionID = ReservedActionID::CONNECT;
					id.SequenceID = 0;

					networkEntityMap[id] = playerEntity;

					// TODO: Set state for user, waiting for user information.
				} return true;

				case ID_DISCONNECTION_NOTIFICATION:
				{
					g_engineContext.m_logger->LogText(LogTag::SERVER, LogLevel::DEBUG_PRINT, "Client disconnected (%s)", p_packet->systemAddress.ToString());
					
					NetworkMessage::UserDisconnected m;
					m.User = m_peer->GetIndexFromSystemAddress(p_packet->systemAddress);
					
					RakNet::BitStream bs;
					bs.Write((RakNet::MessageID) NetworkMessage::MessageType::UserDisconnected);
					m.Serialize(true, &bs);

					// TODO: Send only to clients who have received user connected for all other clients?
					m_peer->Send(&bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_RAKNET_GUID, true);
				} return true;

				case ID_CONNECTION_LOST:
				{
					g_engineContext.m_logger->LogText(LogTag::SERVER, LogLevel::DEBUG_PRINT, "Client disconnected (%s)", p_packet->systemAddress.ToString());
					
					NetworkMessage::UserDisconnected m;
					m.User = m_peer->GetIndexFromSystemAddress(p_packet->systemAddress);
					
					RakNet::BitStream bs;
					bs.Write((RakNet::MessageID) NetworkMessage::MessageType::UserDisconnected);
					m.Serialize(true, &bs);

					// TODO: Send only to clients who have received user connected for all other clients?
					m_peer->Send(&bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_RAKNET_GUID, true);
				} return true;

				case ID_UNCONNECTED_PING:
				{
					g_engineContext.m_logger->LogText(LogTag::SERVER, LogLevel::DEBUG_PRINT, "A LAN-discovery request has been received from %s.", p_packet->systemAddress.ToString());
				} return true;

				case NetworkMessage::MessageType::Chat:
				{
					NetworkMessage::Chat m;
					m.Serialize(false, p_bs);

					g_engineContext.m_logger->LogText(LogTag::SERVER, LogLevel::DEBUG_PRINT, "Chat from client (%s): %s", p_packet->systemAddress.ToString(), m.Message.C_String());

					// Forward the message to broadcast
					RakNet::BitStream bs;
					bs.Write((RakNet::MessageID) NetworkMessage::MessageType::Chat);
					
					m.Sender = (int8_t) m_peer->GetIndexFromSystemAddress(p_packet->systemAddress);
					m.Serialize(true, &bs);

					m_peer->Send(&bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_RAKNET_GUID, true);
				} return true;

				case NetworkMessage::MessageType::UserInformation:
				{
					NetworkMessage::UserInformation m;
					m.Serialize(false, p_bs);

					// Update the player entity
					NetworkEntityID id;
					id.UserID = m_peer->GetIndexFromSystemAddress(p_packet->systemAddress);
					id.ActionID = ReservedActionID::CONNECT;
					id.SequenceID = 0;

					PlayerComponent* player = m_world->GetEntityManager()->CreateComponent<PlayerComponent>(networkEntityMap[id]);
					player->Name = m.Name.C_String();
					
					// Send server information
					RakNet::BitStream bs;
					bs.Write((RakNet::MessageID) NetworkMessage::MessageType::ServerInformation);
					serverInfo->Information.Serialize(true, &bs);

					m_peer->Send(&bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, p_packet->systemAddress, false);

					// TODO: Set state for user, waiting for map load status.
				} return true;

				case NetworkMessage::MessageType::PlayerCommand:
				{
					// TODO: Fix this after connections work.
				} return true;

				case NetworkMessage::MessageType::LoadMapStatus:
				{
					NetworkMessage::LoadMapStatus m;
					m.Serialize(false, p_bs);

					switch (m.Status)
					{
						case NetworkMessage::LoadMapStatus::STATUS_LOADING:
						{
							// TODO: Ignore this for now.
						} break;

						case NetworkMessage::LoadMapStatus::STATUS_COMPLETED:
						{
							g_engineContext.m_logger->LogText(LogTag::SERVER, LogLevel::DEBUG_PRINT, "Client (%s) successfully loaded map", p_packet->systemAddress.ToString());
							
							// Get the player information
							NetworkEntityID id;
							id.UserID = m_peer->GetIndexFromSystemAddress(p_packet->systemAddress);
							id.ActionID = ReservedActionID::CONNECT;
							id.SequenceID = 0;

							PlayerComponent* player = m_world->GetEntityManager()->GetComponent<PlayerComponent>(networkEntityMap[id]);
							
							// Create the player, given a player entity.
							g_engineContext.m_script->SetFunction(g_engineContext.m_resourceManager->GetScript("Player"), "OnCreate");
							g_engineContext.m_script->AddParameterUserData(networkEntityMap[id], sizeof(ECS::Entity*), "Entity");
							g_engineContext.m_script->AddParameterNumber(m_peer->GetIndexFromSystemAddress(p_packet->systemAddress));
							g_engineContext.m_script->AddParameterNumber(Network::ReservedActionID::CONNECT);
							g_engineContext.m_script->ExecuteScript();

							// Send a user connected message to all clients except connectee.
							DataStructures::List<RakNet::SystemAddress> addresses;
							DataStructures::List<RakNet::RakNetGUID> guids;
							m_peer->GetSystemList(addresses, guids);

							for (unsigned int i = 0; i < addresses.Size(); ++i)
							{
								if (addresses[i] != p_packet->systemAddress)
								{
									NetworkMessage::UserConnected n;
									n.User = m_peer->GetIndexFromSystemAddress(p_packet->systemAddress);
									n.IsYou = false;
									n.Name = RakNet::RakString(player->Name.c_str());

									RakNet::BitStream bs;
									bs.Write((RakNet::MessageID) NetworkMessage::MessageType::UserConnected);
									n.Serialize(true, &bs);

									m_peer->Send(&bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, addresses[i], false);
								}
							}

							// Send a user connected message for all clients to the connectee.
							for (unsigned int i = 0; i < addresses.Size(); ++i)
							{
								// Get player information
								NetworkEntityID id;
								id.UserID = i;
								id.ActionID = ReservedActionID::CONNECT;
								id.SequenceID = 0;

								player = m_world->GetEntityManager()->GetComponent<PlayerComponent>(networkEntityMap[id]);

								// Craft message
								NetworkMessage::UserConnected n;
								n.User = m_peer->GetIndexFromSystemAddress(p_packet->systemAddress);
								n.IsYou = n.User == i;
								n.Name = RakNet::RakString(player->Name.c_str());

								RakNet::BitStream bs;
								bs.Write((RakNet::MessageID) NetworkMessage::MessageType::UserConnected);
								n.Serialize(true, &bs);

								m_peer->Send(&bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, p_packet->systemAddress, false);
							}
						} break;

						case NetworkMessage::LoadMapStatus::STATUS_FAILED_MAP_NOT_FOUND:
						{
							g_engineContext.m_logger->LogText(LogTag::SERVER, LogLevel::WARNING, "Client (%s) failed to load map: Map not found", p_packet->systemAddress.ToString());
							m_peer->CloseConnection(p_packet->systemAddress, true);
						} break;

						case NetworkMessage::LoadMapStatus::STATUS_FAILED_INVALID_HASH:
						{
							g_engineContext.m_logger->LogText(LogTag::SERVER, LogLevel::WARNING, "Client (%s) failed to load map: Invalid map hash", p_packet->systemAddress.ToString());
							m_peer->CloseConnection(p_packet->systemAddress, true);
						} break;
					}
				} return true;
			}
			
			return false;
		}
	}
}