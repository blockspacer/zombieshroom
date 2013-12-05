#pragma once

#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <RootEngine/Render/Include/Texture.h>
#include <RootEngine/Render/Include/Renderer.h>
#include <RootEngine/Include/Logging/Logging.h>
#include <memory>

namespace RootEngine
{
	class TextureImporter
	{
	public:
		
		TextureImporter(Logging* p_logger, Render::RendererInterface* p_renderer);
		~TextureImporter();

		bool LoadTexture(const std::string p_fileName);
		Render::TextureInterface* GetTexture();

	private:
		Logging*					m_logger;
		Render::TextureInterface*	m_texture; 
		Render::RendererInterface*	m_renderer;
	};
}