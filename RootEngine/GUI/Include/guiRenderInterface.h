#pragma once
#include <RootEngine/Include/ResourceManager/ResourceManager.h>
#include <Rocket\Core\RenderInterface.h>
#include <Rocket/Core.h>


class guiRenderInterface : public Rocket::Core::RenderInterface
{
public:
	guiRenderInterface(void);
	~guiRenderInterface(void);

	 /**
     * @p width width of viewport
     * @p height height of viewport
     */
    void SetViewport(int width, int height);

	void SetEffect(Render::EffectInterface* p_effect);

	/// Called by Rocket when it wants to render geometry that it does not wish to optimise.
	void RenderGeometry(Rocket::Core::Vertex* vertices, int num_vertices, int* indices, int num_indices, Rocket::Core::TextureHandle texture, const Rocket::Core::Vector2f& translation);

	/// Called by Rocket when it wants to compile geometry it believes will be static for the forseeable future.
	Rocket::Core::CompiledGeometryHandle CompileGeometry(Rocket::Core::Vertex* vertices, int num_vertices, int* indices, int num_indices, Rocket::Core::TextureHandle texture);

	/// Called by Rocket when it wants to render application-compiled geometry.
	void RenderCompiledGeometry(Rocket::Core::CompiledGeometryHandle geometry, const Rocket::Core::Vector2f& translation);
	/// Called by Rocket when it wants to release application-compiled geometry.
	void ReleaseCompiledGeometry(Rocket::Core::CompiledGeometryHandle geometry);

	/// Called by Rocket when it wants to enable or disable scissoring to clip content.
	void EnableScissorRegion(bool enable);
	/// Called by Rocket when it wants to change the scissor region.
	void SetScissorRegion(int x, int y, int width, int height);

	/// Called by Rocket when a texture is required by the library.
	bool LoadTexture(Rocket::Core::TextureHandle& texture_handle, Rocket::Core::Vector2i& texture_dimensions, const Rocket::Core::String& source);
	/// Called by Rocket when a texture is required to be built from an internally-generated sequence of pixels.
	bool GenerateTexture(Rocket::Core::TextureHandle& texture_handle, const Rocket::Core::byte* source, const Rocket::Core::Vector2i& source_dimensions);
	/// Called by Rocket when a loaded texture is no longer required.
	void ReleaseTexture(Rocket::Core::TextureHandle texture_handle);

private:
	int m_width;
	int m_height;

	Render::EffectInterface* m_effect;
};
