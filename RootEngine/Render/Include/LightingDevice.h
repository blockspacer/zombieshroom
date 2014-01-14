#pragma once

#include <RootEngine\Render\Include\Mesh.h>
#include <RootEngine\Render\Include\Effect.h>
#include <RootEngine\Render\Include\Light.h>

#define RENDER_MAX_DIRECTIONALLIGHTS 16
#define RENDER_MAX_POINTLIGHTS 16

namespace Render
{
	class GLRenderer;

	class LightingDevice
	{
	public:
		LightingDevice();
		void Init(GLRenderer* p_renderer, int p_width, int p_height);

		void SetAmbientLight(const glm::vec4& p_color);
		void AddDirectionalLight(const DirectionalLight& p_light, int index);
		void AddPointLight(const PointLight& p_light, int index);

		void Process();
		void Resize(int p_width, int p_height);

	private:

		Mesh m_fullscreenQuad;
		Buffer m_lights;

		struct
		{
			glm::vec4 m_ambient;
			DirectionalLight m_dlights[RENDER_MAX_DIRECTIONALLIGHTS];
			PointLight m_plights[RENDER_MAX_POINTLIGHTS];
		
		} m_lightVars;

		size_t m_numDirectionalLights;
		size_t m_numPointLights;

		std::shared_ptr<TechniqueInterface> m_lightingTech;

		GLuint m_fbo;
		GLuint m_laHandle;
	};
}