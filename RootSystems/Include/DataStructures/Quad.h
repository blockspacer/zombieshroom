#pragma once

#include <Utility/ECS/Include/World.h>
#include <RootSystems/Include/Shapes/AABB.h>
#include <RootEngine/Include/GameSharedContext.h>
#include <RootEngine/Render/Include/Mesh.h>
#include <vector>

#define QUADTREE_TRIANGLES_PER_NODE 3000
#define QUAD_MAX_CHILDS 4

namespace RootForce
{
	class QuadNode
	{
	public:

		friend class QuadTree;

		QuadNode(AABB& p_bounds, unsigned p_numTriangles);
		~QuadNode();

		void AddChild(QuadNode* p_child);
		const std::vector<QuadNode*>& GetChilds();
		const std::vector<ECS::Entity*>& GetEntities();
		const AABB& GetBounds() const;

	private:

		AABB m_bounds;
		unsigned m_numTriangles;
		std::vector<QuadNode*> m_childs;
		std::vector<ECS::Entity*> m_entities;
	};

	struct MeshData
	{
		std::vector<unsigned int> m_indices;
		std::vector<glm::vec3> m_positions;
	};

	class QuadTree
	{
	public:
		QuadTree();
		void Init(RootEngine::GameSharedContext* p_context, ECS::World* p_world);

		QuadNode* PickRoot(glm::vec2 p_position);
		void RenderDebug();
		QuadNode* GetRoot() const;

	private:
		
		QuadNode* PickNode(QuadNode* p_node, glm::vec2 p_position);
		void RenderNode(QuadNode* p_node);	
		int RoundToPow2(int p_value);
		void Subdivide( QuadNode* p_node );
		int CountTriangles( Rectangle p_rect );
		bool IsTriangleContained( glm::vec3 p_vertices[3], Rectangle p_rect ); 

		RootEngine::GameSharedContext* m_context;
		ECS::World* m_world;
		int m_minY, m_maxY;

		QuadNode* m_root;
		std::vector<std::pair<ECS::Entity*, MeshData>> m_globalEntityList;

		int m_split;
	};
}