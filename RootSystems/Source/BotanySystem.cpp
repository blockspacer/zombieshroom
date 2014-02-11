#include <RootSystems/Include/BotanySystem.h>
#include <RootEngine/Render/Include/Renderer.h>
#include <RootEngine/Include/ResourceManager/ResourceManager.h>

namespace RootForce
{
	void BotanySystem::Initialize()
	{
		// Load rendering effects.
		m_effect = m_engineContext->m_resourceManager->LoadEffect("Botany");

		// Calculate AABB of painted entities.
		// TODO: Export bounds.
		/*CalculatePaintedAABB();

		m_width = m_aabb.m_maxX - m_aabb.m_minX;
		m_height = m_aabb.m_maxZ - m_aabb.m_minZ;

		m_view = glm::lookAt(glm::vec3(0,1,0), glm::vec3(0,0,0), glm::vec3(0,0,1));

		glm::vec4 min = glm::vec4(m_aabb.m_minX, m_aabb.m_minY, m_aabb.m_minZ, 1.0f);
		glm::vec4 max = glm::vec4(m_aabb.m_maxX, m_aabb.m_maxY, m_aabb.m_maxZ, 1.0f);

		min.x = -(int)pow(2, ceil(log(glm::abs(min.x))/log(2)));
		min.z = -(int)pow(2, ceil(log(glm::abs(min.z))/log(2)));

		max.x = (int)pow(2, ceil(log(max.x)/log(2)));
		max.z = (int)pow(2, ceil(log(max.z)/log(2)));

		m_proj = glm::ortho(min.x, max.x, min.z, max.z, -max.y, -min.y);

		int plantGridWidth = m_width / BOTANY_CELL_SIZE;
		int plantGridHeight = m_height / BOTANY_CELL_SIZE;

		m_floatDistrubution = std::uniform_real_distribution<float>(0.0f, 1.0f);*/
	}

	void BotanySystem::SetQuadTree(QuadTree* p_quadTree)
	{
		m_quadTree = p_quadTree;
	}


	void BotanySystem::CalculatePaintedAABB()
	{
		// Get working set.
		ECS::GroupManager::GroupRange range = m_world->GetGroupManager()->GetEntitiesInGroup("Painted");

		std::vector<glm::vec3> vertices;
		for(auto itr = range.first; itr != range.second; ++itr)
		{
			ECS::Entity* entity = (*itr).second;

			RootForce::Renderable* renderable = m_world->GetEntityManager()->GetComponent<RootForce::Renderable>(entity);
			auto material = renderable->m_material;
			auto mesh = renderable->m_model->m_meshes[0];

			RootForce::Transform* transform = m_world->GetEntityManager()->GetComponent<RootForce::Transform>(entity);
			glm::mat4x4 transformMatrix;
			transformMatrix = glm::translate(glm::mat4(1.0f), transform->m_position);
			transformMatrix = glm::rotate(transformMatrix, transform->m_orientation.GetAngle(), transform->m_orientation.GetAxis());
			transformMatrix = glm::scale(transformMatrix, transform->m_scale);

			// Parse vertex data.
			glBindBuffer(GL_ARRAY_BUFFER, mesh->GetVertexBuffer()->GetBufferId());
			unsigned char* data = (unsigned char*)glMapBuffer(GL_ARRAY_BUFFER, GL_READ_ONLY);

			int offset = 0;
			for(unsigned i = 0; i < mesh->GetVertexBuffer()->GetBufferSize(); i += mesh->GetVertexBuffer()->GetElementSize())
			{
				Render::Vertex1P1N1UV1T1BT v;
				v.m_tangent = glm::vec3(0);
				v.m_bitangent = glm::vec3(0);
				memcpy(&v, &data[i], mesh->GetVertexBuffer()->GetElementSize());

				// Transform vertex to world space.
				glm::vec4 tf = transformMatrix * glm::vec4(v.m_pos, 1.0f);
				v.m_pos = glm::vec3(tf.x, tf.y, tf.z);

				vertices.push_back(v.m_pos);
				offset++;
			}

			glUnmapBuffer(GL_ARRAY_BUFFER);
		}

		float maxX, maxY, maxZ, minX, minY, minZ;
		maxX = maxY = maxZ = -99999;
		minX = minY = minZ = 99999;

		for(auto itr = vertices.begin(); itr != vertices.end(); ++itr)
		{
			glm::vec3 pos = (*itr);
			for(int i = 0; i < 3; i++)
			{
				float tx = pos.x;
				float ty = pos.y;
				float tz = pos.z;

				if(tx > maxX) maxX = tx;
				if(tx < minX) minX = tx;
				
				if(ty > maxY) maxY = ty;
				if(ty < minY) minY = ty;

				if(tz > maxZ) maxZ = tz;
				if(tz < minZ) minZ = tz;	
			}
		}

		m_aabb.m_maxX = maxX;
		m_aabb.m_minX = minX;
		m_aabb.m_maxY = maxY;
		m_aabb.m_minY = minY;
		m_aabb.m_maxZ = maxZ;
		m_aabb.m_minZ = minZ;
	}

	void BotanySystem::DensityRenderToTexture(RootForce::RenderingSystem* m_renderingSystem)
	{
		// Create RTT.
		m_density = m_engineContext->m_renderer->CreateRenderToTexture();
		m_density->SetTexture(m_engineContext->m_renderer->CreateTexture());
		m_density ->GetTexture()->CreateEmptyTexture(1024, 1024, Render::TextureFormat::TEXTURE_RGB);
		m_density->BindTexture();

		// Set Camera.
		m_engineContext->m_renderer->SetViewMatrix(m_view);
		m_engineContext->m_renderer->SetProjectionMatrix(m_proj);

		// Draw painted geometry.
		ECS::GroupManager::GroupRange range = m_world->GetGroupManager()->GetEntitiesInGroup("Painted");
		for(auto itr = range.first; itr != range.second; ++itr)
		{
			ECS::Entity* entity = (*itr).second;

			RootForce::Renderable* renderable = m_world->GetEntityManager()->GetComponent<RootForce::Renderable>(entity);
			auto material = renderable->m_material;
			auto mesh = renderable->m_model->m_meshes[0];

			RootForce::Transform* transform = m_world->GetEntityManager()->GetComponent<RootForce::Transform>(entity);

			// Calculate model/normal.
			m_matrices[entity].m_model = glm::translate(glm::mat4(1.0f), transform->m_position);
			m_matrices[entity].m_model = glm::rotate(m_matrices[entity].m_model, transform->m_orientation.GetAngle(), transform->m_orientation.GetAxis());
			m_matrices[entity].m_model = glm::scale(m_matrices[entity].m_model, transform->m_scale);
			m_matrices[entity].m_normal = glm::mat4(glm::transpose(glm::inverse(glm::mat3(m_matrices[entity].m_model))));
			m_matrices[entity].m_model = m_matrices[entity].m_model;

			Render::RenderJob job;
			job.m_flags = renderable->m_renderFlags;
			job.m_mesh = renderable->m_model->m_meshes[0];
			job.m_shadowMesh = renderable->m_model->m_meshes[0];
			job.m_params = renderable->m_params;
			job.m_params[Render::Semantic::MODEL] = &m_matrices[entity].m_model;
			job.m_params[Render::Semantic::NORMAL] = &m_matrices[entity].m_normal;
			job.m_material = renderable->m_material;
			job.m_renderPass = renderable->m_pass;

			m_engineContext->m_renderer->AddRenderJob(job);
		}

		m_engineContext->m_renderer->SetRenderToTexture(m_density);
		m_engineContext->m_renderer->Render();

		// Store texture.
		m_density->Store("rtt.tga");

		// Restore backbuffer.
		m_engineContext->m_renderer->SetRenderToTexture(nullptr);

		/*range = m_world->GetGroupManager()->GetEntitiesInGroup("Painted");
		for(auto itr = range.first; itr != range.second; ++itr)
		{
			ECS::Entity* entity = (*itr).second;

			RootForce::Renderable* renderable = m_world->GetEntityManager()->GetComponent<RootForce::Renderable>(entity);
			auto material = renderable->m_material;
			auto mesh = renderable->m_model->m_meshes[0];

			material->m_textures[Render::TextureSemantic::TEXTUREMAP] = m_density->GetTexture();
		}

		m_world->GetEntityManager()->GetComponent<RootForce::Renderable>(m_world->GetTagManager()->GetEntityByTag("Player"))->m_material->m_textures[Render::TextureSemantic::DIFFUSE] = m_density->GetTexture();*/
	}

	void BotanySystem::Process()
	{
		m_aabb.DebugDraw(m_engineContext->m_renderer, glm::vec3(1,1,0), glm::mat4(1.0f));

		// Get eye camera.
		ECS::Entity* entity = m_world->GetTagManager()->GetEntityByTag("Camera");
		RootForce::Frustum* frustrum = &m_world->GetEntityManager()->GetComponent<RootForce::Camera>(entity)->m_frustum;
		RootForce::Transform* transform = m_world->GetEntityManager()->GetComponent<RootForce::Transform>(entity);

		m_quadTree->m_culledNodes.clear();

		// Cull terrain chunks.
		m_quadTree->CullNodes(frustrum, m_quadTree->GetRoot());

		for(auto itr = m_quadTree->m_culledNodes.begin(); itr != m_quadTree->m_culledNodes.end(); ++itr)
		{
			AABB bounds = (*itr)->GetBounds();
			glm::vec3 center = bounds.GetCenter();
			float d = glm::distance(transform->m_position, center);
			
			for(auto entity = (*itr)->m_terrainEntityIndices.begin(); entity != (*itr)->m_terrainEntityIndices.end(); ++entity)
			{
				RootForce::Renderable* renderable = m_world->GetEntityManager()->GetComponent<RootForce::Renderable>(m_quadTree->m_entities[(*entity)]);

				Render::RenderJob job;
				job.m_mesh = renderable->m_model->m_meshes[0];
				
				job.m_material = renderable->m_material;
				job.m_flags = renderable->m_renderFlags;
				job.m_renderPass = renderable->m_pass;
				job.m_params = renderable->m_params;

				m_engineContext->m_renderer->AddRenderJob(job);
			}
		}


		/*
		RootForce::Transform* transform = m_world->GetEntityManager()->GetComponent<RootForce::Transform>(m_world->GetTagManager()->GetEntityByTag("Player"));

		float u = (transform->m_position.x - m_aabb.m_minX) / m_width;
		float v = (transform->m_position.z - m_aabb.m_minZ) / m_height;
		
		std::cout << "U: " << 1 - u << "V: " << v << std::endl;
		*/
	}
}