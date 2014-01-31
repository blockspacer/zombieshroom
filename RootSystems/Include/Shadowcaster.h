#pragma once

#include <Utility/ECS/Include/Component.h>
#include <RootSystems/Include/Frustum.h>

namespace RootForce
{
	struct Shadowcaster : public ECS::Component<Shadowcaster>
	{
		Shadowcaster(){}
		//Frustum m_frustum;
		int m_levels; //For cascaded shadow-mapping
		std::vector<glm::mat4> m_cascades;
	};
}