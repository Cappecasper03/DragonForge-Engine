#include "cFramebuffer_vulkan.h"

#include <glm/vec2.hpp>
#include <tracy/Tracy.hpp>
#include <vulkan/vulkan.hpp>

#include "engine/rendering/cRenderer.h"
#include "engine/rendering/iRenderer.h"

namespace df::vulkan
{
	cFramebuffer_vulkan::cFramebuffer_vulkan( std::string _name, const unsigned /*_num_render_textures*/, const bool /*_generate_render_buffer*/, const glm::ivec2& _size )
		: iFramebuffer( std::move( _name ) )
	{
		ZoneScoped;

		glm::ivec2 window_size = _size;
		if( window_size.x < 0 || window_size.y < 0 )
			window_size = cRenderer::getRenderInstance()->getWindowSize();
	}

	cFramebuffer_vulkan::~cFramebuffer_vulkan()
	{
		ZoneScoped;
	}
}