#include <RootEngine/Include/TextureImporter.h>

namespace RootEngine
{
	
	TextureImporter::TextureImporter(Logging* p_logger, Render::RendererInterface* p_renderer)
	{
		m_logger	= p_logger;
		m_renderer  = p_renderer;
		m_logger->LogText(LogTag::RESOURCE, LogLevel::INIT_PRINT, "Texture importer initialized!");
	}

	TextureImporter::~TextureImporter()
	{
		if(m_texture)
			delete m_texture;
	}

	bool TextureImporter::LoadTexture( const std::string p_fileName )
	{
		m_texture   = m_renderer->CreateTexture();
		if (m_texture->Load(p_fileName)) 
			return true;
		else 
			return false;
	}

	Render::TextureInterface* TextureImporter::GetTexture()
	{
		return m_texture;
	}



}