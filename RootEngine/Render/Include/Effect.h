#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <memory>
#include <map>

#include <RootEngine\Render\Include\Semantics.h>
#include <RootEngine\Render\Include\Buffer.h>

namespace Render
{
	class RendererInterface;

	class Shader
	{
	public:
		Shader();
		~Shader();
		GLint LoadFromFile( GLenum p_shaderType, const char* p_filename );
		GLuint GetHandle() { return m_glHandle; }
	private:
		GLuint m_glHandle;
	};

	class ProgramInterface
	{
	public:
		virtual void CreateProgram() = 0;
		virtual GLint AttachShader( GLenum p_shaderType, const char* p_filename ) = 0;
		virtual GLint Compile() = 0;
		virtual void Apply() = 0;
		virtual GLuint GetHandle() = 0;
		virtual void BindUniformBuffer(const std::string& bufferName, unsigned int slot) = 0;
		virtual void BindTexture(const std::string& textureName, unsigned int slot) = 0;
	};

	class Program : public ProgramInterface
	{
	public:

		enum BlendState { BLEND_NONE, BLEND_ALPHA, BLEND_ADDITIVE, BLEND_INV_ALPHA };

		enum FillMode { FILL_SOLID, FILL_WIREFRAME };

		struct DepthState {		
			bool depthTest;
			bool depthWrite;
		};

		Program();
		~Program();

		void CreateProgram();

		GLint AttachShader( GLenum p_shaderType, const char* p_filename );
		GLint Compile();

		void Apply();
		GLuint GetHandle();

		void BindUniformBuffer(const std::string& bufferName, unsigned int slot);
		void BindTexture(const std::string& textureName, unsigned int slot);

		BlendState m_blendState;
		DepthState m_depthState;
		FillMode m_fillMode;

	private:
		GLuint m_glHandle;
	};

	class TechniqueInterface
	{
	public:
		virtual std::shared_ptr<Program> CreateProgram() = 0;
		virtual std::vector<std::shared_ptr<Program>>& GetPrograms() = 0;
		virtual std::shared_ptr<Render::BufferInterface> GetUniformBuffer() = 0;
		virtual void Apply() = 0;
	};

	class Technique : public TechniqueInterface
	{
	public:
		std::shared_ptr<Program> CreateProgram();
		std::vector<std::shared_ptr<Program>>& GetPrograms();

		std::shared_ptr<Render::BufferInterface> GetUniformBuffer();
		void Apply();

		/* 
			How per object uniforms should work.

		   .vert
			perObject { float4x4 g_matrix; }

		   technique
			BONES - 0

		   material
			addParam(BONES, &m_boneMatrix);

		   rendering
			foreach param
				read offset from technique using semantic
					bufferSubData(offset, pointer)
		*/
		std::map<Semantic::Semantic, unsigned int> m_data;
		std::shared_ptr<Render::BufferInterface> m_perTechniqueBuffer;
		
	private:
		std::vector<std::shared_ptr<Program>> m_program;
		
	};

	class EffectInterface
	{
	public:
		virtual std::shared_ptr<Technique> CreateTechnique(RendererInterface* p_renderer) = 0;
		virtual std::vector<std::shared_ptr<Technique>>& GetTechniques() = 0;
	};

	class Effect : public EffectInterface
	{
	public:
		std::shared_ptr<Technique> CreateTechnique(RendererInterface* p_renderer);
		std::vector<std::shared_ptr<Technique>>& GetTechniques();

	private:
		std::vector<std::shared_ptr<Technique>> m_techniques;

	};

	struct EffectParamsInterface
	{
		virtual void AllocateParams(EffectInterface* p_effect) = 0;
	};

	struct EffectParams : public EffectParamsInterface
	{
		void AllocateParams(EffectInterface* p_effect);
	};
}
