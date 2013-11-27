
#include <GL/glew.h>

#include <RootEngine/Include/Logging/Logging.h>
#include <RootEngine/Include/ResourceManager/ResourceManager.h>

#include <RootEngine/Render/Include/Renderer.h>
#include <RootEngine/Render/Include/RenderExtern.h>

#if defined(_DEBUG) && defined(WIN32)
#include <windows.h>
void APIENTRY PrintOpenGLError(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, GLvoid* param) 
{
	Render::g_context.m_logger->LogText("message: %s", message);
	Render::g_context.m_logger->LogText("type: ");
	switch (type) {
	case GL_DEBUG_TYPE_ERROR:
		Render::g_context.m_logger->LogText("ERROR");
		break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
		Render::g_context.m_logger->LogText("DEPRECATED_BEHAVIOR");
		break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
		Render::g_context.m_logger->LogText("UNDEFINED_BEHAVIOR");
		break;
	case GL_DEBUG_TYPE_PORTABILITY:
		Render::g_context.m_logger->LogText("PORTABILITY");
		break;
	case GL_DEBUG_TYPE_PERFORMANCE:
		Render::g_context.m_logger->LogText("PERFORMANCE");
		break;
	case GL_DEBUG_TYPE_OTHER:
		Render::g_context.m_logger->LogText("OTHER");
		break;
	}

	Render::g_context.m_logger->LogText("id: %i", id);
	Render::g_context.m_logger->LogText("severity: ");
	switch (severity)
	{
	case GL_DEBUG_SEVERITY_LOW:
		Render::g_context.m_logger->LogText("LOW");
		break;
	case GL_DEBUG_SEVERITY_MEDIUM:
		Render::g_context.m_logger->LogText("MEDIUM");
		break;
	case GL_DEBUG_SEVERITY_HIGH:
		Render::g_context.m_logger->LogText("HIGH");
		break;
	}
}
#endif

namespace Render
{
	RootEngine::SubsystemSharedContext g_context;

	GLRenderer::GLRenderer()
	{
		
	}

	GLRenderer::~GLRenderer()
	{

	}

	void GLRenderer::Startup()
	{

	}

	void GLRenderer::Shutdown()
	{
		SDL_GL_DeleteContext(m_glContext);
	}

	void GLRenderer::SetupSDLContext(SDL_Window* p_window)
	{
		int flags = SDL_GL_CONTEXT_PROFILE_CORE;
#if defined (_DEBUG)
		flags |= SDL_GL_CONTEXT_DEBUG_FLAG;
#endif

		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, flags);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);

		m_glContext = SDL_GL_CreateContext(p_window);
		if(!m_glContext) {
			g_context.m_logger->LogText("%s", SDL_GetError());
		}

		SDL_GL_SetSwapInterval(0);

		int width, height;
		SDL_GetWindowSize(p_window, &width, &height);
		glViewport(0, 0, width, height);

		// Init GLEW.
		glewExperimental = GL_TRUE; 
		GLenum err = glewInit();
		if (err != GLEW_OK) {
			Render::g_context.m_logger->LogText(LogTag::RENDER,  LogLevel::FATAL_ERROR, "Failed to initialize glew!");
			return;
		}

		GLint major, minor;
		glGetIntegerv(GL_MAJOR_VERSION, &major);
		glGetIntegerv(GL_MINOR_VERSION, &minor);
		Render::g_context.m_logger->LogText(LogTag::RENDER,  LogLevel::DEBUG_PRINT, "OpenGL context version: %d.%d", major, minor);

		glClearColor(0,0,0,1);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glEnable(GL_DEPTH_TEST);
		glFrontFace(GL_CCW);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

#if defined(_DEBUG) && defined(WIN32)
		if(glDebugMessageCallbackARB)
		{
			g_context.m_logger->LogText(LogTag::RENDER,  LogLevel::DEBUG_PRINT, "Register OpenGL debug callback ");
			glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
			glDebugMessageCallbackARB(PrintOpenGLError, nullptr);
			GLuint unusedIds = 0;
			glDebugMessageControlARB(GL_DONT_CARE,
				GL_DONT_CARE,
				GL_DONT_CARE,
				0,
				&unusedIds,
				true);
		}
		else
			g_context.m_logger->LogText(LogTag::RENDER,  LogLevel::NON_FATAL_ERROR, "glDebugMessageCallback not available");
#endif

		// Check for extensions.
		if(CheckExtension("GL_NVX_gpu_memory_info"))
		{
			g_context.m_logger->LogText(LogTag::RENDER,  LogLevel::DEBUG_PRINT, "Available video memory: %i KB", GetAvailableVideoMemory());
		}

		CheckExtension("NV_texture_multisample");

		m_window = p_window;

		m_gbuffer.Init(width, height);

		// Setup fullscreen quad.

		Render::Vertex1P1UV verts[4];
		verts[0].m_pos = glm::vec3(-1.0f, -1.0f, 0.0f);
		verts[1].m_pos = glm::vec3(+1.0f, -1.0f, 0.0f);
		verts[2].m_pos = glm::vec3(-1.0f, +1.0f, 0.0f);
		verts[3].m_pos = glm::vec3(+1.0f, +1.0f, 0.0f);

		verts[0].m_UV = glm::vec2(0.0f, 0.0f);
		verts[1].m_UV = glm::vec2(1.0f, 0.0f);
		verts[2].m_UV = glm::vec2(0.0f, 1.0f);
		verts[3].m_UV = glm::vec2(1.0f, 1.0f);
		
		unsigned int indices[6];
		indices[0] = 0; 
		indices[1] = 1; 
		indices[2] = 2;
		indices[3] = 2;
		indices[4] = 1; 
		indices[5] = 3;

		m_fullscreenQuad.Init(verts, 4, indices, 6);
		
		// Load effects.

		g_context.m_resourceManager->LoadEffect("Output");
		m_output = g_context.m_resourceManager->GetEffect("Output");
	
		g_context.m_resourceManager->LoadEffect("Deferred");
		m_deferred = g_context.m_resourceManager->GetEffect("Deferred");
	
		m_lightingTech = m_deferred->GetTechniques()[0];

		// Setup camera.

		m_camera.Initialize(glm::vec3(0,0,10), glm::vec3(0), glm::vec3(0,1,0), 45.0f, 1.0f, 100.0, width, height);
		
		m_cameraVars.m_projection = m_camera.GetProjection();
		m_cameraVars.m_view = m_camera.GetView();
		
		m_cameraBuffer.Init(GL_UNIFORM_BUFFER);
		m_cameraBuffer.BufferData(1, sizeof(m_cameraVars), &m_cameraVars);

		// Setup uniforms buffers.

		m_uniforms.Init(GL_UNIFORM_BUFFER);

		m_lights.Init(GL_UNIFORM_BUFFER);
		m_lightVars.m_direction = glm::vec3(0,0,-1);
		m_lightVars.m_ambient = glm::vec3(0.3f,0.3f,0.3f);
		m_lights.BufferData(1, sizeof(m_lightVars), &m_lightVars);
	}

	void GLRenderer::SetResolution(int p_width, int p_height)
	{
		/*SDL_SetWindowFullscreen(m_window, SDL_WINDOW_FULLSCREEN_DESKTOP);
		int width;
		int height;
		SDL_GetWindowSize(m_window, &width, &height);*/

		SDL_SetWindowSize(m_window, p_width, p_height);
		
		m_gbuffer.Resize(p_width, p_height);

		glViewport(0, 0, p_width, p_height);

		m_camera.PerspectiveProjection(45.0f, 1.0f, 100.0, p_width, p_height);
	}

	void GLRenderer::AddRenderJob(RenderJob* p_job)
	{
		m_jobs.push_back(p_job);
	}

	void GLRenderer::Render()
	{
		Clear();

		// Geometry pass.
		m_gbuffer.Bind();

		for(auto itr = m_jobs.begin(); itr != m_jobs.end(); ++itr)
		{
			m_uniforms.BufferData(1, sizeof(Uniforms), (*itr)->m_uniforms);

			for(auto itrT = (*itr)->m_effect->GetTechniques().begin(); itrT != (*itr)->m_effect->GetTechniques().end(); ++itrT)
			{
				for(auto itrP = (*itrT)->GetPrograms().begin(); itrP != (*itrT)->GetPrograms().end(); ++itrP)
				{
					(*itrP)->Apply();

					(*itrP)->SetUniformBuffer(m_cameraBuffer.GetBufferId(), "PerFrame", 0);
					(*itrP)->SetUniformBuffer(m_uniforms.GetBufferId(), "PerObject", 1);

					(*itr)->m_mesh->Bind();
					(*itr)->m_mesh->DrawArrays();
					(*itr)->m_mesh->Unbind();
				}
			}
		}

		m_jobs.clear();

		m_gbuffer.Unbind(); // Unbind GBuffer and restore backbuffer.
		m_gbuffer.Read(); // Enable the GBuffer for reads.

		// Lighting pass.

		for(auto itr = m_lightingTech->GetPrograms().begin(); itr != m_lightingTech->GetPrograms().end(); ++itr)
		{
			(*itr)->Apply();

			(*itr)->SetTexture(m_gbuffer.m_diffuse.GetHandle(), "g_Diffuse", 0);
			(*itr)->SetTexture(m_gbuffer.m_normals.GetHandle(), "g_Normals", 1);

			(*itr)->SetUniformBuffer(m_lights.GetBufferId(), "Lights", 2);

			m_fullscreenQuad.Bind();

			m_fullscreenQuad.DrawInstanced(1);

			m_fullscreenQuad.Unbind();
		}

		// Output.

		Swap();
	}

	void GLRenderer::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void GLRenderer::Swap()
	{
		SDL_GL_SwapWindow(m_window);
	}

	bool GLRenderer::CheckExtension(const char* p_extension)
	{
		if(glewIsExtensionSupported(p_extension) == GL_FALSE) {
			g_context.m_logger->LogText(LogTag::RENDER,  LogLevel::WARNING,  "Missing OpenGL extension: %s", p_extension);
			return false;
		}
		return true;
	}

	int GLRenderer::GetAvailableVideoMemory()
	{
		GLint cur_avail_mem_kb = 0;
		glGetIntegerv(GL_GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX, &cur_avail_mem_kb);

		return cur_avail_mem_kb;
	}
}

Render::RendererInterface* CreateRenderer(RootEngine::SubsystemSharedContext p_context)
{
	Render::g_context = p_context;

	return new Render::GLRenderer;
}
