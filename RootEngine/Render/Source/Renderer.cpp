
#include <GL/glew.h>
#include <RootEngine/Include/Logging/Logging.h>
#include <RootEngine/Render/Include/Renderer.h>
#include <RootEngine/Render/Include/Shader.h>
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

	GLRenderer* GLRenderer::s_rendererInstance = nullptr;

	GLRenderer::GLRenderer()
	{
		
	}

	GLRenderer::~GLRenderer()
	{

	}

	GLRenderer* GLRenderer::GetInstance()
	{
		if(!s_rendererInstance)
		{
			s_rendererInstance = new GLRenderer();
		}

		return s_rendererInstance;
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
		glEnable(GL_DEPTH_TEST);
		glCullFace(GL_FRONT);
		glFrontFace(GL_CW);
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

		m_camera.Initialize(glm::vec3(0,0,10), glm::vec3(0), glm::vec3(0,1,0), 45.0f, 1.0f, 100.0);
		
		m_cameraVars.m_projection = m_camera.GetProjection();
		m_cameraVars.m_view = m_camera.GetView();
		
		m_cameraBuffer.Init(GL_UNIFORM_BUFFER);
		m_cameraBuffer.BufferData(1, sizeof(m_cameraVars), &m_cameraVars);


		//m_effect.Apply();

		m_uniforms.Init(GL_UNIFORM_BUFFER);

		m_lights.Init(GL_UNIFORM_BUFFER);
		m_lightVars.m_direction = glm::vec3(0,0,-1);
		m_lights.BufferData(1, sizeof(m_lightVars), &m_lightVars);

		//m_effect.SetUniformBuffer(m_camerBuffer.GetBufferId(), "PerFrame", 0);			
		//m_effect.SetUniformBuffer(m_uniforms.GetBufferId(), "PerObject", 1);
		//m_effect.SetUniformBuffer(m_lights.GetBufferId(), "Lights", 2);
	}

	void GLRenderer::AddRenderJob(RenderJob* p_job)
	{
		m_jobs.push_back(p_job);
	}

	void GLRenderer::Render()
	{
		Clear();

		for(auto itr = m_jobs.begin(); itr != m_jobs.end(); ++itr)
		{
			m_uniforms.BufferData(1, sizeof(Uniforms), (*itr)->m_uniforms);
			//m_effect.SetUniformBuffer(m_uniforms.GetBufferId(), "PerObject", 1);


			(*itr)->m_effect->Apply();
			(*itr)->m_effect->SetUniformBuffer(m_uniforms.GetBufferId(), "PerObject", 1);
			(*itr)->m_effect->SetUniformBuffer(m_cameraBuffer.GetBufferId(), "PerFrame", 0);
			(*itr)->m_effect->SetUniformBuffer(m_lights.GetBufferId(), "Lights", 2);
			(*itr)->m_mesh->Bind();
			(*itr)->m_mesh->DrawArrays();
			(*itr)->m_mesh->Unbind();
		}

		m_jobs.clear();

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

	void GLRenderer::DrawLine(std::vector<glm::vec3> p_debugVectors)
	{
		int size  = p_debugVectors.size();
		if(size <= 0)
			return;

		
		Vertex1P1C* vertices = new Render::Vertex1P1C();
		unsigned int* indices = (unsigned int*)malloc(sizeof(unsigned int) * size);

		/*m_debugVectors.push_back(from);
		m_debugVectors.push_back(to);
		m_debugVectors.push_back(color);*/
		for(int i = 0; i < size; i+=2)
		{
			vertices[i].m_pos = p_debugVectors[i];
			vertices[i].m_color = glm::vec4(1.0f, 0, 0, 1.0f);
			indices[i] = i;
		}
		int hurdeur = 3;
		//vertices[0].m_pos = p_pos1;
		//vertices[0].m_color = glm::vec4(p_colour, 1.0f);
		//vertices[1].m_pos = p_pos2;
		//vertices[1].m_color = glm::vec4(p_colour, 1.0f);

		

		Uniforms uniforms;
		uniforms.m_normal = glm::mat4(1);
		uniforms.m_world = glm::mat4(1);

		std::shared_ptr<MeshInterface> mesh = CreateMesh();
		mesh->Init(vertices, size, indices, size);

		//RenderJob job;
		//job.m_mesh = mesh;
		//job.m_uniforms = &uniforms;
		//job.m_effect = 0; //GIVE US OUR RESOURCE MANAGER GODDAMNIT!!

		//AddRenderJob(&job);
		
	}

}

Render::RendererInterface* CreateRenderer(RootEngine::SubsystemSharedContext p_context)
{
	Render::g_context = p_context;

	return Render::GLRenderer::GetInstance();
}
