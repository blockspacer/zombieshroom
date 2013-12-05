#include <Utility\ECS\Include\EntitySystem.h>
#include <Utility\ECS\Include\World.h>

void ECS::EntitySystem::Process()
{
	Begin();

	for(auto itr = m_activeEntities.begin(); itr != m_activeEntities.end(); ++itr)
	{
		ProcessEntity((*itr));
	}

	End();	
}

bool ECS::IntervalEntitySystem::CheckProcessing()
{
	float dt = m_world->GetDelta();
	m_time += dt;
	if(m_time >= m_interval)
	{
		m_time = 0.0f;
		return true;
	}
	return false;
}

void ECS::IntervalEntitySystem::Process()
{
	if(CheckProcessing())
	{
		Begin();

		for(auto itr = m_activeEntities.begin(); itr != m_activeEntities.end(); ++itr)
		{
			ProcessEntity((*itr));
		}

		End();
	}
}