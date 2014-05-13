#include <RootEngine/Include/OBB.h>
#include <cstdlib>
#include <glm/gtc/swizzle.hpp>

namespace RootEngine
{
	OBB::OBB()
		: m_minX(999999), m_maxX(-999999), m_minY(999999), m_maxY(-999999), m_minZ(999999), m_maxZ(-999999)
	{}

	OBB::OBB(float p_minX, float p_maxX, float p_minY, float p_maxY, float p_minZ, float p_maxZ, glm::mat4 p_transform)
		: m_minX(p_minX), m_maxX(p_maxX), m_minY(p_minY), m_maxY(p_maxY), m_minZ(p_minZ), m_maxZ(p_maxZ), m_transform(p_transform)
	{}

	void OBB::Expand(const glm::vec3& p_expansion)
	{
		if(p_expansion.x > m_maxX)
		{
			m_maxX = p_expansion.x;
		}
		if(p_expansion.x < m_minX)
		{
			m_minX = p_expansion.x;
		}
		if(p_expansion.y > m_maxY)
		{
			m_maxY = p_expansion.y;
		}
		if(p_expansion.y < m_minY)
		{
			m_minY = p_expansion.y;
		}
		if(p_expansion.z > m_maxZ)
		{
			m_maxZ = p_expansion.z;
		}
		if(p_expansion.z < m_minZ)
		{
			m_minZ = p_expansion.z;
		}
	}

	void OBB::DebugDraw(Render::RendererInterface* p_renderer, glm::vec3& p_color, glm::mat4 p_space) const
	{
		glm::vec4 positions[8];
		positions[0] = p_space * m_transform * glm::vec4(m_minX, m_minY, m_minZ, 1.0f);
		positions[1] = p_space * m_transform * glm::vec4(m_maxX, m_minY, m_minZ, 1.0f);
		positions[2] = p_space * m_transform * glm::vec4(m_minX, m_minY, m_maxZ, 1.0f);
		positions[3] = p_space * m_transform * glm::vec4(m_maxX, m_minY, m_maxZ, 1.0f);
		positions[4] = p_space * m_transform * glm::vec4(m_minX, m_maxY, m_minZ, 1.0f);
		positions[5] = p_space * m_transform * glm::vec4(m_maxX, m_maxY, m_minZ, 1.0f);
		positions[6] = p_space * m_transform * glm::vec4(m_minX, m_maxY, m_maxZ, 1.0f);
		positions[7] = p_space * m_transform * glm::vec4(m_maxX, m_maxY, m_maxZ, 1.0f);

		unsigned int indices[] = 
		{ 
		  0, 2, 2, 3, 3, 1, 1, 0,
		  4, 6, 6, 7, 7, 5, 5, 4,
		  0, 4, 2, 6, 3, 7, 1, 5 
		};

		for(int i = 0; i < 24; i += 2)
		{
			glm::vec3 pos1, pos2;
			pos1 = glm::swizzle<glm::X, glm::Y, glm::Z>(positions[indices[i]]);
			pos2 = glm::swizzle<glm::X, glm::Y, glm::Z>(positions[indices[i+1]]);
			p_renderer->AddLine(pos1, pos2, glm::vec4(p_color.x, p_color.y, p_color.z, 1.0f));
		}
	}
}

		