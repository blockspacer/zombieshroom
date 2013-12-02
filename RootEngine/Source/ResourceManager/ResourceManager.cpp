#include <RootEngine/Include/ResourceManager/ResourceManager.h>

namespace RootEngine
{

	ResourceManager::ResourceManager()
	{
	}

	ResourceManager::~ResourceManager()
	{
#ifndef COMPILE_LEVEL_EDITOR
		auto modelitr = m_models.begin();
		for(; modelitr != m_models.end(); modelitr++)
		{
			delete (*modelitr).second;
			(*modelitr).second = nullptr;
		}
		m_models.clear();

		auto meshitr = m_meshes.begin();
		for(; meshitr != m_meshes.end(); meshitr++)
		{
			delete (*meshitr).second;
			(*meshitr).second = nullptr;
		}
		m_meshes.clear();
#endif

		auto effectitr = m_effects.begin();
		for(; effectitr != m_effects.end(); effectitr++)
		{
			delete (*effectitr).second;
			(*effectitr).second = nullptr;
		}
		m_effects.clear();

	}

	void ResourceManager::Init(std::string p_workingDirectory, Render::RendererInterface* p_renderer, Logging* p_logger)
	{
		m_logger			= p_logger;
		m_workingDirectory	= p_workingDirectory;

		m_effectImporter = std::shared_ptr<EffectImporter>(new EffectImporter(p_renderer));
		m_effectImporter->SetWorkingDirectory(m_workingDirectory);

#ifndef COMPILE_LEVEL_EDITOR
		m_modelImporter = std::shared_ptr<ModelImporter>(new ModelImporter(m_logger, p_renderer));
#endif
	}

#ifndef COMPILE_LEVEL_EDITOR
	void ResourceManager::LoadCollada(std::string p_path)
	{
		m_models[p_path] = m_modelImporter->LoadModel(m_workingDirectory + "Assets\\Models\\" + p_path + "\\" + p_path + ".DAE");
	}
#endif

	void ResourceManager::LoadEffect(std::string p_path)
	{
		m_effectImporter->Load(m_workingDirectory + "Assets//Shaders//" + p_path + ".effect");
		m_effects[p_path] = m_effectImporter->m_effect;
	}

	Render::EffectInterface* ResourceManager::GetEffect(std::string p_handle)
	{
		return m_effects[p_handle];
	}

#ifndef COMPILE_LEVEL_EDITOR
	Model* ResourceManager::GetModel( std::string p_handle )
	{
		return m_models[p_handle];
	}
#endif

}