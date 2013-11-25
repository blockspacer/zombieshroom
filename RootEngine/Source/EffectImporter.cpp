#include <RootEngine/Include/EffectImporter.h>

namespace RootEngine
{
	void EffectImporter::Process(const YAML::Node& p_node)
	{
		std::shared_ptr<Render::EffectInterface> effect = m_renderer->CreateEffect();
		
		std::string techName;
		std::string param;

		// Parsing technique.
		p_node["name"] >> techName;
		p_node["param"] >> param;

		std::cout << techName << std::endl;
		std::cout << param << std::endl;

		effect->CreateEffect();

		const YAML::Node& passes = p_node["passes"];
		for(int i = 0; i < passes.size(); ++i)
		{
			std::string shaderName;
			passes[i]["name"] >> shaderName;

			// Parse type of shader.
			std::string type;
			passes[i]["type"] >> type;
			
			GLenum glType;
			std::string extension;
			if(type == "vertex") 
			{
				glType = GL_VERTEX_SHADER;
				extension = ".vert";
			}
			else if(type == "fragment")
			{
				glType = GL_FRAGMENT_SHADER;
				extension = ".frag";
			}

			std::string shader = std::string(m_workingDirectory + "Assets//Shaders//" + shaderName  + extension);
			effect->AttachShader(glType, shader.c_str());

			std::cout << shaderName << std::endl;
			std::cout << type << std::endl;
		}

		effect->Compile();
		m_effect = effect;
	}

	void EffectImporter::SetWorkingDirectory(std::string p_workingDirectory)
	{
		m_workingDirectory = p_workingDirectory;
	}
}