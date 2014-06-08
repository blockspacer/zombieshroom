#include <RootTools/Treenity/Include/Shaders/DiffuseNormalTransView.h>

extern RootEngine::GameSharedContext g_engineContext;

DiffuseNormalTransShaderView::DiffuseNormalTransShaderView(QWidget* p_parent)
	: AbstractShaderView(p_parent)
{
	ui.setupUi(this);
	//Init alltexture drops with Texture semantic and a name
	ui.textureDrop_diffuse->Init(Render::TextureSemantic::DIFFUSE, "Diffuse");
	ui.textureDrop_specular->Init(Render::TextureSemantic::SPECULAR, "Specular");
	ui.textureDrop_normal->Init(Render::TextureSemantic::NORMAL, "Normal");
	ui.textureDrop_glow->Init(Render::TextureSemantic::GLOW, "Glow");
	ui.textureDrop_trans->Init(Render::TextureSemantic::TRANSLUCENCY, "Translucency");

	//Push them to the texture drop list, if they are not in this list they will not work
	m_textureDropList.push_back(ui.textureDrop_diffuse);
	m_textureDropList.push_back(ui.textureDrop_specular);
	m_textureDropList.push_back(ui.textureDrop_normal);
	m_textureDropList.push_back(ui.textureDrop_glow);
	m_textureDropList.push_back(ui.textureDrop_trans);
	
	//Connect all texture drops to the texture changed slot
	for(auto texDrop : m_textureDropList)
		connect(texDrop, SIGNAL(textureChanged(Render::TextureInterface*, Render::TextureSemantic::TextureSemantic)), this, SLOT(TextureChanged(Render::TextureInterface*, Render::TextureSemantic::TextureSemantic)));

}

void DiffuseNormalTransShaderView::SetEngineInterface(EngineInterface* p_engineInterface)
{
	//Set Engine interface for all texture drops
	for(auto tex : m_textureDropList)
	{
		tex->SetEngineInterface(p_engineInterface);
	}

	AbstractShaderView::SetEngineInterface(p_engineInterface);
}

void DiffuseNormalTransShaderView::Display()
{
	//This will loop through all texture drops and display an image if there is one
	for(auto tex : m_textureDropList)
	{
		auto itr = m_material->m_textures.find(tex->GetTextureSemantic());
		if(itr != m_material->m_textures.end())
		{
			if(itr->second == nullptr)
			{
				g_engineContext.m_logger->LogText(LogTag::GUI, LogLevel::NON_FATAL_ERROR, "Received nullptr %s texture", tex->GetName().toStdString().c_str());
			}
			else
			{
				tex->SetTexture(itr->second);
			}
		}
		else
		{
			tex->Clear();
		}
	}
}

void DiffuseNormalTransShaderView::TextureChanged( Render::TextureInterface* p_texture, Render::TextureSemantic::TextureSemantic p_sem )
{
	m_material->m_textures[p_sem] = p_texture;
	Save();
}
