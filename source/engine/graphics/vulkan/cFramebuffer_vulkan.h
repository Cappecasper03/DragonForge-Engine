#pragma once

#include <string>
#include <vulkan/vulkan.hpp>

#include "engine/core/math/cVector.h"
#include "engine/misc/Misc.h"
#include "graphics/api/iFramebuffer.h"
#include "types/sAllocatedImage_vulkan.h"

namespace df::vulkan
{
	class cFramebuffer_vulkan : public iFramebuffer
	{
	public:
		DF_DisableCopyAndMove( cFramebuffer_vulkan );

		explicit cFramebuffer_vulkan( std::string _name, uint32_t _num_render_textures = 0, uint32_t _frames_in_flight = 1, const cVector2i& _size = cVector2i( -1, -1 ) );
		~cFramebuffer_vulkan() override = default;

		const std::vector< sAllocatedImage_vulkan >& getCurrentFrameImages( const uint32_t _frame_index ) const { return m_frame_images[ _frame_index ]; }

	private:
		std::vector< std::vector< sAllocatedImage_vulkan > > m_frame_images;
	};
}
