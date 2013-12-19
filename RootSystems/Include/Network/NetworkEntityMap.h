#pragma once

#include <map>
#include <stdint.h>
#include <Utility/ECS/Include/EntitySystem.h>

namespace RootForce
{
	namespace Network
	{
		typedef int16_t TemporaryId_t;
		typedef int16_t SynchronizedId_t;

		const TemporaryId_t TEMPORARY_ID_NONE = -1;
		const SynchronizedId_t SYNCHRONIZED_ID_NONE = -1;

		class NetworkEntityMap
		{
		public:
			NetworkEntityMap();

			TemporaryId_t AddEntity(ECS::Entity* p_entity);
			void SetSynchronizedId(TemporaryId_t p_temporaryId, SynchronizedId_t p_synchronizedId);
			void RemoveSynchronizedEntity(SynchronizedId_t p_synchronizedId);

			SynchronizedId_t GetSynchronizedId(ECS::Entity* p_entity) const;
			TemporaryId_t GetTemporaryId(ECS::Entity* p_entity) const;
			ECS::Entity* GetSynchronizedEntity(SynchronizedId_t p_synchronizedId) const;

			/** Only used by server */
			SynchronizedId_t NextSynchronizedId();
		private:
			std::map<TemporaryId_t, ECS::Entity*> m_temporaryEntityMap;
			std::map<SynchronizedId_t, ECS::Entity*> m_synchronizedEntityMap;

			TemporaryId_t m_nextTemporaryId;
			SynchronizedId_t m_nextSynchronizedId;
		};
	}
}