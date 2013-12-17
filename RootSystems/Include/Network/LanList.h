#pragma once

#include <RootSystems/Include/Network/ServerInfo.h>
#include <stdint.h>
#include <vector>
#include <utility>

namespace RootSystems
{
	class LanList
	{
	public:
		void Start();
		void AddServer(RootSystems::ServerInfoInternal* p_server);
		std::vector<std::pair<uint64_t,RootSystems::ServerInfoInternal*>> GetList();

		static bool ServerCompare(std::pair<uint64_t,RootSystems::ServerInfoInternal*> &first, std::pair<uint64_t,RootSystems::ServerInfoInternal*> &second);

	private:
		uint64_t m_pingStart;
		std::vector<std::pair<uint64_t,RootSystems::ServerInfoInternal*>> m_list;
	};
}