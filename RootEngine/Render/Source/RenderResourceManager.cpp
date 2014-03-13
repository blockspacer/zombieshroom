#include <RootEngine/Render/Include/RenderResourceManager.h>
#include <RenderExtern.h>

namespace Render
{
	RenderResourceManager::RenderResourceManager()
	{
		m_buffers.reserve(10000);
		m_textures.reserve(1000);
		m_materials.reserve(10000);
		m_meshes.reserve(10000);
		m_vaos.reserve(10000);
		m_effects.reserve(1000);
	}

	RenderResourceManager::~RenderResourceManager()
	{
		g_context.m_logger->LogText(LogTag::RESOURCE, LogLevel::DEBUG_PRINT, "Cleaning all render resources.");
		Clean(RenderResources::RR_ALL);
	}

	void RenderResourceManager::PrintResourceUsage(int& p_bufferUsage, int& p_textureUsage, int& p_numBuffers, int& p_numTextures)
	{
		p_bufferUsage = 0;
		int numBuffers = 0;
		for(auto itr = m_buffers.begin(); itr != m_buffers.end(); ++itr)
		{
			if((*itr) != nullptr)
			{
				p_bufferUsage += (*itr)->GetBufferSize();
				numBuffers++;
			}
		}
		p_bufferUsage /= 1000;

		p_textureUsage = 0;
		for(auto itr = m_textures.begin(); itr != m_textures.end(); ++itr)
		{
			int w = (*itr)->GetWidth();
			int h = (*itr)->GetHeight();

			for(int i = 0; i < (*itr)->GetMipsLevels(); ++i)
			{
				p_textureUsage += (w * h * (*itr)->GetBytesPerPixel()) / (*itr)->GetCompressRatio();		
				w /= 2;
				h /= 2;
			}
		}
		p_textureUsage /= 1000;

		p_numBuffers = numBuffers;
		p_numTextures = m_textures.size();
	}

	BufferInterface* RenderResourceManager::CreateBuffer(GLenum p_type)
	{ 
		Buffer* buffer = new Buffer(p_type);
		m_buffers.push_back(buffer);
		return buffer;
	}

	void RenderResourceManager::ReleaseBuffer(BufferInterface* p_buffer)
	{
		auto itr = std::find(m_buffers.begin(), m_buffers.end(), p_buffer);
		if(itr != m_buffers.end())
		{
			if((*itr) != nullptr)
			{
				g_context.m_logger->LogText(LogTag::RESOURCE, LogLevel::DEBUG_PRINT, "Release buffer %d", p_buffer->GetBufferId());
				delete (*itr);
				(*itr) = nullptr;
			}

			m_buffers.erase(itr);
		}		
	}

	TextureInterface* RenderResourceManager::CreateTexture()
	{
		Texture* texture = new Texture();
		m_textures.push_back(texture);
		return texture;
	}

	void RenderResourceManager::ReleaseTexture(TextureInterface* p_texture)
	{
		auto itr = std::find(m_textures.begin(), m_textures.end(), p_texture);
		if(itr != m_textures.end())
		{
			if((*itr) != nullptr)
			{
				g_context.m_logger->LogText(LogTag::RESOURCE, LogLevel::DEBUG_PRINT, "Release texture %d", p_texture->GetHandle());
				delete (*itr);
				(*itr) = nullptr;
			}

			m_textures.erase(itr);
		}	
	}

	Material* RenderResourceManager::CreateMaterial(const std::string& p_name)
	{
		auto itr = m_materialNameMap.find(p_name);
		if(itr == m_materialNameMap.end())
		{
			m_materialNameMap[p_name] = m_materials.size();

			Material mat = Material(m_materials.size());
			m_materials.push_back(std::move(mat));
			return &m_materials[m_materials.size()-1];
		}

		return &m_materials[(*itr).second];
	}

	VertexAttributesInterface* RenderResourceManager::CreateVertexAttributes()
	{ 
		VertexAttributes* vao = new VertexAttributes();
		m_vaos.push_back(vao);
		return vao;
	}

	MeshInterface* RenderResourceManager::CreateMesh() 
	{ 
		Mesh* mesh = new Mesh();
		m_meshes.push_back(mesh);
		return mesh;
	}

	EffectInterface* RenderResourceManager::CreateEffect() 
	{ 
		Effect* effect = new Effect();
		m_effects.push_back(effect);
		return effect;
	}

	std::string RenderResourceManager::GetStringFromMaterial(Material* p_material)
	{
		for(auto itr = m_materialNameMap.begin(); itr != m_materialNameMap.end(); ++itr)
		{
			if((*itr).second == p_material->m_id)
				return (*itr).first;
		}
		return std::string("");
	}

	void RenderResourceManager::RemoveMesh(MeshInterface* p_mesh)
	{
		auto itr = std::find(m_meshes.begin(), m_meshes.end(), p_mesh);
		if(itr != m_meshes.end())
		{
			g_context.m_logger->LogText(LogTag::RESOURCE, LogLevel::DEBUG_PRINT, "Remove Mesh.");
			(*itr)->FreeBuffers(this);
			if((*itr) != nullptr)
			{
				delete (*itr);
				(*itr) = nullptr;
			}

			m_meshes.erase(itr);
		}	
	}

	void RenderResourceManager::RemoveVAO(VertexAttributesInterface* p_vao)
	{	
		auto itr = std::find(m_vaos.begin(), m_vaos.end(), p_vao);
		if(itr != m_vaos.end())
		{
			if((*itr) != nullptr)
			{
				g_context.m_logger->LogText(LogTag::RESOURCE, LogLevel::DEBUG_PRINT, "Remove VAO.");
				delete (*itr);
				(*itr) = nullptr;
			}
			m_vaos.erase(itr);
		}		
	}

	void RenderResourceManager::Clean(unsigned p_cleanFlag)
	{
		if((p_cleanFlag & RenderResources::RR_BUFFER) == RenderResources::RR_BUFFER)
		{
			for(auto itr = m_buffers.begin(); itr != m_buffers.end(); ++itr)
				delete (*itr);
			m_buffers.clear();
		}

		if((p_cleanFlag & RenderResources::RR_TEXTURE) == RenderResources::RR_TEXTURE)
		{
			for(auto itr = m_textures.begin(); itr != m_textures.end(); ++itr)
				delete (*itr);
			m_textures.clear();
		}

		if((p_cleanFlag & RenderResources::RR_MESH) == RenderResources::RR_MESH)
		{
			for(auto itr = m_meshes.begin(); itr != m_meshes.end(); ++itr)
				delete (*itr);
			m_meshes.clear();
		}

		if((p_cleanFlag & RenderResources::RR_VAO) == RenderResources::RR_VAO)
		{
			for(auto itr = m_vaos.begin(); itr != m_vaos.end(); ++itr)
				delete (*itr);
			m_vaos.clear();
		}

		if((p_cleanFlag & RenderResources::RR_EFFECT) == RenderResources::RR_EFFECT)
		{
			for(auto itr = m_effects.begin(); itr != m_effects.end(); ++itr)
				delete (*itr);
			m_effects.clear();
		}

		if((p_cleanFlag & RenderResources::RR_MATERIAL) == RenderResources::RR_MATERIAL)
		{
			m_materials.clear();
		}
	}
}