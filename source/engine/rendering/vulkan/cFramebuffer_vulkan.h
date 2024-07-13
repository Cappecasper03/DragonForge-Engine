#pragma once

#include <glm/vec2.hpp>
#include <string>

#include "engine/misc/Misc.h"
#include "engine/rendering/iFramebuffer.h"

namespace df::vulkan
{
	class cFramebuffer_vulkan : public iFramebuffer
	{
	public:
		DF_DISABLE_COPY_AND_MOVE( cFramebuffer_vulkan )

		explicit cFramebuffer_vulkan( std::string _name, unsigned _num_render_textures = 0, bool _generate_render_buffer = true, const glm::ivec2& _size = glm::ivec2( -1, -1 ) );
		~cFramebuffer_vulkan() override;
	};
}
