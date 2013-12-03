#include "NetworkManager.h"
#include "LocalServer.h"
#include "RemoteServer.h"
#include <iostream>
#include <gtest/gtest.h>
namespace RootEngine
{
	namespace Network
	{
		SubsystemSharedContext g_context;
		NetworkManager* NetworkManager::s_networkManager = nullptr;
		void NetworkManager::Startup()
		{
			g_context.m_logger->LogText(LogTag::NETWORK, LogLevel::DEBUG_PRINT, "Succesfull startup of Network");
		}

		void NetworkManager::Shutdown()
		{
			delete m_networkSys;
			delete s_networkManager;
		}

		void NetworkManager::Initialize(PeerType::PeerType p_peerType)
		{
			switch( p_peerType )
			{
			case PeerType::LOCALSERVER:
				m_networkSys = new LocalServer();
				g_context.m_logger->LogText(LogTag::NETWORK, LogLevel::DEBUG_PRINT, "Local Server created successfully");
				break;
			case PeerType::REMOTESERVER:
				m_networkSys = new RemoteServer();
				g_context.m_logger->LogText(LogTag::NETWORK, LogLevel::DEBUG_PRINT, "Remote Server created successfully");
				break;
			default:
				g_context.m_logger->LogText(LogTag::NETWORK, LogLevel::NON_FATAL_ERROR, "Network manager was fed undefined peerType, no network created");
				break;
			}
		}

		NetworkManager* NetworkManager::GetInstance()
		{
			if(!s_networkManager)
				s_networkManager = new NetworkManager();

			return s_networkManager;
		}

		
		
	}
}

RootEngine::Network::NetworkInterface* CreateNetwork(RootEngine::SubsystemSharedContext p_context)
{ 
	RootEngine::Network::g_context = p_context;

	return RootEngine::Network::NetworkManager::GetInstance();
}

TEST(NETWORK, NETWORK_SEND)
{
	RootEngine::Network::Message testMessage;
	testMessage.Data = (RootEngine::Network::byte*)"I AM A POTATOE GOD";
	testMessage.DataSize = 19;
	testMessage.MessageID = 0;
	testMessage.RecipientID = -1;
	testMessage.Reliable = true;
	EXPECT_TRUE(RootEngine::Network::NetworkManager::GetInstance()->GetNetworkSystem()->Send( testMessage ));
}
